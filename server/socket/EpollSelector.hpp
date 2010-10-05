
#ifndef __EPOLLSELECTOR_HPP__
# define __EPOLLSELECTOR_HPP__

# include <map>
# include <sys/epoll.h>

# include "api/types.hpp"

# include "SocketEvent.hpp"
# include "ISocketEventNotifier.hpp"
# include "Socket.hpp"

namespace ZHTTPD
{
    namespace POLICIES
    {
        class EpollSelector
        {
            typedef struct epoll_event epoll_event_t;

        private:
            ZHTTPD::API::socket_t  _epfd;          // epoll file descriptor
            ZHTTPD::API::size_t    _nfds;          // number of monitored file descriptors
            ZHTTPD::API::size_t    _max_events;    // events tab size
            epoll_event_t*      _events;        // events tab
            ZHTTPD::Socket*        _write_interrupt_socket;
            ZHTTPD::Socket*        _read_interrupt_socket;

        public:
            static ZHTTPD::API::size_t const EPOLL_SIZE;

        private:
            void _initInterruptors();

        public:
            EpollSelector();
            virtual ~EpollSelector();
            void watchSockets(ISocketEventNotifier& notifier);
            void registerFileDescriptor(ZHTTPD::API::socket_t fd);
            void unregisterFileDescriptor(ZHTTPD::API::socket_t fd);
            void interruptWatch();
        };

        typedef EpollSelector Selector;
    }
}

#endif /* !__EPOLLSELECTOR_HPP__ */

