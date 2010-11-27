
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

namespace zhttpd
{
    namespace POLICIES
    {
        class BasicSocketSelector
        {
            typedef std::set<api::socket_t> fdset_t;

        private:
            zhttpd::Socket*        _write_interrupt_socket;
            zhttpd::Socket*        _read_interrupt_socket;
            fd_set              _sets[2];
            zhttpd::api::socket_t  _max_fd;
            fdset_t             _fdlist;
            Mutex               _mutex;

        public:
            BasicSocketSelector();
            ~BasicSocketSelector();
            void watchSockets(ISocketEventNotifier& notifier);
            void interruptWatch();

            void registerFileDescriptor(zhttpd::api::socket_t fd);
            void unregisterFileDescriptor(zhttpd::api::socket_t fd);

        private:
            void _prepareSockets(ISocketEventNotifier& notifier, fdset_t& fdlist);
            void _updateEvents(ISocketEventNotifier& notifier, fdset_t& fdlist);
            void _initInterruptors();
        };

        typedef BasicSocketSelector Selector;
    }
}

#endif /* !__BASICSOCKETSELECTOR_HPP__ */
