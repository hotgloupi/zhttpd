
#ifndef __EPOLLSELECTOR_HPP__
# define __EPOLLSELECTOR_HPP__

# include <map>
# include <sys/epoll.h>

# include "api/types.hpp"

# include "SocketEvent.hpp"
# include "ISocketEventNotifier.hpp"
# include "Socket.hpp"

namespace zhttpd
{
    namespace POLICIES
    {
        class EpollSelector
        {
            typedef struct epoll_event epoll_event_t;

        private:
            zhttpd::api::socket_t  _epfd;          // epoll file descriptor
            zhttpd::api::size_t    _nfds;          // number of monitored file descriptors
            zhttpd::api::size_t    _max_events;    // events tab size
            epoll_event_t*      _events;        // events tab
            zhttpd::Socket*        _write_interrupt_socket;
            zhttpd::Socket*        _read_interrupt_socket;

        public:
            static zhttpd::api::size_t const EPOLL_SIZE;

        private:
            void _initInterruptors();

        public:
            EpollSelector();
            virtual ~EpollSelector();
            void watchSockets(ISocketEventNotifier& notifier);
            void registerFileDescriptor(zhttpd::api::socket_t fd);
            void unregisterFileDescriptor(zhttpd::api::socket_t fd);
            void interruptWatch();
        };

        typedef EpollSelector Selector;
    }
}

#endif /* !__EPOLLSELECTOR_HPP__ */

