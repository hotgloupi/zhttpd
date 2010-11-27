
#ifndef __SOCKETPOOL_HPP__
# define __SOCKETPOOL_HPP__

# include <cassert>
# include <list>
# include <map>
# include <set>

# include "thread/ITask.hpp"
# include "thread/Mutex.hpp"
# include "thread/ScopeLock.hpp"
# include "utils/Logger.hpp"
# include "utils/Timer.hpp"
# include "utils/macros.hpp"
# include "core/ServerState.hpp"

# include "SocketEvent.hpp"
# include "ISocketEventNotifier.hpp"
# include "Socket.hpp"

namespace zhttpd
{

    template<typename SocketSelector>
    class _SocketPool :
        public SocketSelector,
        public ITask,
        public ISocketEventNotifier
    {

    private:
        SocketEvent**   _events;
        Mutex           _events_mutex;
        enum
        {
            EVENTS_SIZE = 65536
        };

    public:
        _SocketPool() : SocketSelector(), _events(0)
        {
            this->_events = new SocketEvent*[EVENTS_SIZE];
            ::memset(this->_events, 0, sizeof(*this->_events) * EVENTS_SIZE);
        }

        virtual ~_SocketPool()
        {
            LOG_INFO("Ending socket pool");
            for (unsigned int i = 0; i < EVENTS_SIZE; ++i)
            {
                ZHTTPD_DELETE(this->_events[i]);
            }
            delete [] this->_events;
        }

        void registerHandler(api::socket_t fd, ISocketEventHandler& object)
        {
            ScopeLock sl(this->_events_mutex);
            if (this->_events[fd] == 0)
            {
                this->_events[fd] = new SocketEvent(object);
            }
            else
            {
                this->_events[fd]->setHandler(object);
            }
            SocketSelector::registerFileDescriptor(fd);
        }

        void unregisterHandler(api::socket_t fd)
        {
            assert(this->_events[fd] != 0);
            if (this->_events[fd]->isEnabled())
                SocketSelector::unregisterFileDescriptor(fd);
            this->_events[fd]->disable();
        }

        void destroyHandler(api::socket_t fd)
        {
            ScopeLock sl(this->_events_mutex);
            assert(this->_events[fd] != 0);
            if (this->_events[fd]->isEnabled())
                SocketSelector::unregisterFileDescriptor(fd);
            this->_events[fd]->destroy();
        }

        void notify(api::socket_t fd, socket_event::Type evt)
        {
            assert(this->_events[fd] != 0);
            this->_events[fd]->notify(evt);
            this->_events[fd]->trigger();
        }

        virtual void run()
        {
            ServerState *sv = ServerState::getInstance();

            while (sv->getState() == ServerState::RUNNING)
            {
                SocketSelector::watchSockets(*this);
            }
        }

        void stop()
        {
            SocketSelector::interruptWatch();
        }
    };
}

# ifdef __linux__
#  include "EpollSelector.hpp"
# else
#  include "BasicSocketSelector.hpp"
# endif

namespace zhttpd
{
    typedef _SocketPool<POLICIES::Selector> SocketPool;
}

#endif /* !__SOCKETPOOL_HPP__ */

