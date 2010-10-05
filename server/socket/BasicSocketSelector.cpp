
#include <stdexcept>
#include <cstring>
#include <errno.h>
#include <cstdio>

#ifndef WIN32
# include <sys/time.h>
#endif // !_WIN32


#include "utils/Logger.hpp"
#include "utils/macros.hpp"
#include "socket/BasicSocketSelector.hpp"
#include "socket/SelfConnector.hpp"
#include "thread/Thread.hpp"
#include "socket/ISocketEventNotifier.hpp"

using namespace ZHTTPD::POLICIES;

BasicSocketSelector::BasicSocketSelector() :
    _write_interrupt_socket(0),
    _read_interrupt_socket(0)
{
    this->_initInterruptors();
}

BasicSocketSelector::~BasicSocketSelector()
{
    ZHTTPD_DELETE(this->_write_interrupt_socket);
    ZHTTPD_DELETE(this->_read_interrupt_socket);
}

void BasicSocketSelector::watchSockets(ZHTTPD::ISocketEventNotifier& notifier)
{
    FD_ZERO(&this->_sets[ZHTTPD::SOCKET_EVENT::CAN_READ]);
    FD_ZERO(&this->_sets[ZHTTPD::SOCKET_EVENT::CAN_WRITE]);
    this->_max_fd = 0;

    FD_SET(this->_read_interrupt_socket->getSocket(),
           &this->_sets[ZHTTPD::SOCKET_EVENT::CAN_READ]);
    this->_max_fd = this->_read_interrupt_socket->getSocket();
    ZHTTPD_LOCK(this->_mutex);
    fdset_t fdlist = this->_fdlist;
    ZHTTPD_UNLOCK(this->_mutex);

    this->_prepareSockets(notifier, fdlist);

    int res = select(this->_max_fd + 1,
                     &this->_sets[ZHTTPD::SOCKET_EVENT::CAN_READ],
                     &this->_sets[ZHTTPD::SOCKET_EVENT::CAN_WRITE],
                     0,
                     0);
#ifndef _WIN32
    if (res < 0 && errno != EINTR)
    {
        LOG_ERROR("select() failure: " + std::string(strerror(errno)));
        return;
    }
#else
    if (res == SOCKET_ERROR)
    {
        LOG_ERROR(std::string("select() failure with error: ") + ::WSAGetLastError());
        return ;
    }
#endif // !WIN32

    if (FD_ISSET(this->_read_interrupt_socket->getSocket(),
                 &this->_sets[ZHTTPD::SOCKET_EVENT::CAN_READ]))
    {
        char buf[4096];
        this->_read_interrupt_socket->read(buf, 4096);
    }

    this->_updateEvents(notifier, fdlist);
}

void BasicSocketSelector::registerFileDescriptor(ZHTTPD::API::socket_t fd)
{
    ZHTTPD_LOCK(this->_mutex);
    if (this->_fdlist.find(fd) == this->_fdlist.end())
    {
        this->_fdlist.insert(fd);
        this->interruptWatch();
    }
    ZHTTPD_UNLOCK(this->_mutex);
}

void BasicSocketSelector::unregisterFileDescriptor(ZHTTPD::API::socket_t fd)
{
    ZHTTPD_LOCK(this->_mutex);
    if (this->_fdlist.find(fd) != this->_fdlist.end())
    {
        this->_fdlist.erase(fd);
        //this->interruptWatch();
    }
    ZHTTPD_UNLOCK(this->_mutex);
}

#ifdef _WIN32
#define PREPARE_SOCKET(event)                                               \
        if (fd > this->_max_fd)                                             \
            this->_max_fd = fd;                                             \
        FD_SET(fd, &this->_sets[event]);
#else // _WIN32
#define PREPARE_SOCKET(event)                                               \
        if (fd > this->_max_fd)                                             \
        {                                                                   \
            if (((unsigned int) fd) >= (FD_SETSIZE - 1))                    \
            {                                                               \
                LOG_WARN("fd = " + Logger::toString(fd) +                   \
                         " >= " + Logger::toString(FD_SETSIZE - 1));        \
                notifier.notify(fd, SOCKET_EVENT::HAS_ERROR);               \
                continue;                                                   \
            }                                                               \
            this->_max_fd = fd;                                             \
        }                                                                   \
        FD_SET(fd, &this->_sets[event]);
#endif // !_WIN32

void BasicSocketSelector::_prepareSockets(ZHTTPD::ISocketEventNotifier& notifier,
                                          fdset_t& fdlist)
{
    ZHTTPD::API::socket_t fd;

    fdset_t::iterator it = fdlist.begin();
    fdset_t::iterator end = fdlist.end();
    for (; it != end ; ++it)
    {
        fd = *it;
        PREPARE_SOCKET(ZHTTPD::SOCKET_EVENT::CAN_READ)
        PREPARE_SOCKET(ZHTTPD::SOCKET_EVENT::CAN_WRITE)
    }
}

#undef PREPARE_SOCKET



void BasicSocketSelector::_updateEvents(ZHTTPD::ISocketEventNotifier& notifier,
                                        fdset_t& fdlist)
{
    ZHTTPD::API::socket_t fd;

    fdset_t::iterator it = fdlist.begin();
    fdset_t::iterator end = fdlist.end();
    for (; it != end ; ++it)
    {
        fd = *it;

#define UPDATE_EVENT(event)                          \
        if (FD_ISSET(fd, &this->_sets[event]))       \
        {                                            \
            notifier.notify(fd, event);              \
        }                                            \

        UPDATE_EVENT(ZHTTPD::SOCKET_EVENT::CAN_READ)
        UPDATE_EVENT(ZHTTPD::SOCKET_EVENT::CAN_WRITE)
#undef UPDATE_EVENT
    }
}

void BasicSocketSelector::_initInterruptors()
{
    bool ok = false;
    for (API::uint16_t port = 9000 ; port < 10000 && ok == false ; ++port)
    {
        try
        {
            ZHTTPD::Socket sl(0x7F000001, port);
            ZHTTPD::SelfConnector connector(port);

            {
                ZHTTPD::Thread t(&connector);
                this->_write_interrupt_socket = sl.accept();
                this->_read_interrupt_socket = connector.getSocket();
            }

            ok = true;
        }
        catch (...) {}
    }
    if (ok == false)
        throw std::runtime_error("Cannot init self connector in for the socket pool");
}

void BasicSocketSelector::interruptWatch()
{
    this->_write_interrupt_socket->write("?", 1);
}

