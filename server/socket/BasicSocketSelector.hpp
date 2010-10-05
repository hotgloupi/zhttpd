
#ifndef __BASICSOCKETSELECTOR_HPP__
# define __BASICSOCKETSELECTOR_HPP__

#ifndef WIN32
# include <sys/select.h>
#endif // !WIN32

# include <set>

# include "thread/Mutex.hpp"

# include "Socket.hpp"
# include "SocketEvent.hpp"
# include "ISocketEventNotifier.hpp"

namespace ZHTTPD
{
    namespace POLICIES
    {
        class BasicSocketSelector
        {
            typedef std::set<API::socket_t> fdset_t;

        private:
            ZHTTPD::Socket*        _write_interrupt_socket;
            ZHTTPD::Socket*        _read_interrupt_socket;
            fd_set              _sets[2];
            ZHTTPD::API::socket_t  _max_fd;
            fdset_t             _fdlist;
            Mutex               _mutex;

        public:
            BasicSocketSelector();
            ~BasicSocketSelector();
            void watchSockets(ISocketEventNotifier& notifier);
            void interruptWatch();

            void registerFileDescriptor(ZHTTPD::API::socket_t fd);
            void unregisterFileDescriptor(ZHTTPD::API::socket_t fd);

        private:
            void _prepareSockets(ISocketEventNotifier& notifier, fdset_t& fdlist);
            void _updateEvents(ISocketEventNotifier& notifier, fdset_t& fdlist);
            void _initInterruptors();
        };

        typedef BasicSocketSelector Selector;
    }
}

#endif /* !__BASICSOCKETSELECTOR_HPP__ */
