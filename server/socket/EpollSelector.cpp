
#ifdef __linux__

#include <errno.h>
#include <cstring>
#include <stdexcept>

#include "thread/Thread.hpp"
#include "utils/Atomic.hpp"
#include "utils/macros.hpp"

#include "SelfConnector.hpp"
#include "EpollSelector.hpp"

using namespace zhttpd::POLICIES;

zhttpd::api::size_t const EpollSelector::EPOLL_SIZE = 5000;

EpollSelector::EpollSelector() :
    _epfd(-1),
    _nfds(0),
    _max_events(0),
    _events(0),
    _write_interrupt_socket(0),
    _read_interrupt_socket(0)
{
    this->_epfd = epoll_create(EpollSelector::EPOLL_SIZE);
    if (this->_epfd < 0)
    {
        throw std::runtime_error("epoll_create(): " + std::string(strerror(errno)));
    }
    this->_max_events = 1024;
    this->_events = new epoll_event_t[this->_max_events];
    this->_initInterruptors();
}

EpollSelector::~EpollSelector()
{
    ZHTTPD_DELETE(this->_write_interrupt_socket);
    ZHTTPD_DELETE(this->_read_interrupt_socket);
    delete [] this->_events;
}

void EpollSelector::watchSockets(ISocketEventNotifier& notifier)
{
    static char buf[4096];
    zhttpd::api::socket_t intfd = this->_read_interrupt_socket->getSocket();
#ifdef ZHTTPD_DEBUG
    if (this->_nfds == 0)
    {
        LOG_WARN("This should not happen !");
        return;
    }
#endif
    if (this->_nfds >= EpollSelector::EPOLL_SIZE)
        throw std::runtime_error("Maximum number of watched fd is reached!");

    if (this->_nfds > this->_max_events)
    {
        this->_max_events = this->_nfds * 2;
        if (this->_max_events > EpollSelector::EPOLL_SIZE)
            this->_max_events = EpollSelector::EPOLL_SIZE;
        epoll_event_t* old = this->_events;
        this->_events = new epoll_event_t[this->_max_events];
        delete [] old;
    }

    int nfsd = epoll_wait(this->_epfd, this->_events, this->_max_events, -1);
    if (nfsd < 0)
    {
        //throw std::runtime_error("epoll_wait(): " + std::string(strerror(errno)));
        LOG_ERROR("epoll_wait(): " + std::string(strerror(errno)));
    }
    for (int i = 0; i < nfsd; ++i)
    {
        epoll_event_t* ev = &this->_events[i];
        zhttpd::api::socket_t fd = ev->data.fd;

        if (fd == intfd)
        {
            if (ev->events & EPOLLIN)
            {
                this->_read_interrupt_socket->read(buf, 4096);
            }
        }
        else
        {
            if (ev->events & EPOLLERR || ev->events & EPOLLRDHUP)
            {
                notifier.notify(fd, zhttpd::socket_event::HAS_ERROR);
            }
            else
            {
                if (ev->events & EPOLLIN)
                {
                    notifier.notify(fd, zhttpd::socket_event::CAN_READ);
                }
                if (ev->events & EPOLLOUT)
                {
                    notifier.notify(fd, zhttpd::socket_event::CAN_WRITE);
                }
            }
        }
    }
}

void EpollSelector::registerFileDescriptor(zhttpd::api::socket_t fd)
{
    epoll_event_t ev;
    ::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
    ev.data.fd = fd;
    if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        LOG_ERROR("epoll_ctl(ADD): " + std::string(strerror(errno)));
        return ;
    }
    zhttpd::Atomic::increment(&this->_nfds);
}

void EpollSelector::unregisterFileDescriptor(zhttpd::api::socket_t fd)
{
    static epoll_event_t ev; // linux kernel < 2.6.9 requires non-NULL pointer in event
    if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, &ev) == -1)
    {
       LOG_ERROR("epoll_ctl(DEL): " + std::string(strerror(errno)));
    }
    zhttpd::Atomic::decrement(&this->_nfds);
}

void EpollSelector::_initInterruptors()
{
    bool ok = false;
    for (api::uint16_t port = 9000 ; port < 10000 && ok == false ; ++port)
    {
        try
        {
            zhttpd::Socket sl(0x7F000001, port);
            zhttpd::SelfConnector connector(port);

            {
                zhttpd::Thread t(&connector);
                this->_write_interrupt_socket = sl.accept();
                this->_read_interrupt_socket = connector.getSocket();
            }

            ok = true;
        }
        catch (...) { /* delete ? */}
    }
    if (ok == false)
        throw std::runtime_error("Cannot init self connector in for the socket pool");
    epoll_event_t ev;
    ::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN | EPOLLRDHUP;
    ev.data.fd = this->_read_interrupt_socket->getSocket();
    if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_read_interrupt_socket->getSocket(), &ev) == -1)
        throw std::runtime_error("epoll_ctl(ADD): " + std::string(strerror(errno)));
    this->_nfds++;
}

void EpollSelector::interruptWatch()
{
    this->_write_interrupt_socket->write("?", 1);
}

#endif /* !__linux__ */
