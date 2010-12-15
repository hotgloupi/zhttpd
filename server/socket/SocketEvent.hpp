
#ifndef __SOCKETEVENT_HPP__
# define __SOCKETEVENT_HPP__

# include <cstring>
# include <cassert>

# include "ISocketEventHandler.hpp"
# include "Socket.hpp"

namespace zhttpd
{
    class SocketEvent
    {
    private:
        ISocketEventHandler*    _object;
        volatile bool           _enabled;
        bool                    _events[socket_event::NB_SOCKET_EVENT];
        Mutex                   _mutex;
        Mutex                   _process_mutex;

    public:
        SocketEvent(ISocketEventHandler& object) :
            _object(&object), _enabled(true)
        {
            ::memset(this->_events, 0, sizeof(this->_events));
        }

        bool isEnabled()
        {
            ZHTTPD_LOCK(this->_mutex);
            bool res = this->_enabled;
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }


        // From the socket pool
        void trigger()
        {
            ZHTTPD_LOCK(this->_process_mutex);
            if (!this->_enabled)
            {
                ZHTTPD_UNLOCK(this->_process_mutex);
                return;
            }
            if (this->_events[socket_event::HAS_ERROR])
            {
                ::memset(this->_events, 0, sizeof(this->_events));
                if (this->_enabled)
                {
                    this->_object->handleSocketEvent(socket_event::HAS_ERROR);
                }
            }
            else
            {
                for (unsigned int i = 0; i < socket_event::NB_SOCKET_EVENT - 1; ++i)
                {
                    if (this->_events[i] == true)
                    {
                        this->_events[i] = false;
                        if (this->_enabled && this->_object != 0)
                        {
                            this->_object->handleSocketEvent(static_cast<socket_event::Type>(i));
                        }
                    }
                }
            }
            ZHTTPD_UNLOCK(this->_process_mutex);
        }

        inline void setHandler(ISocketEventHandler& handler)
        {
            ZHTTPD_LOCK(this->_mutex);
            this->_object = &handler;
            this->_enabled = true;
            ZHTTPD_UNLOCK(this->_mutex);
        }

        inline void disable()
        {
            ZHTTPD_LOCK(this->_mutex);
            this->_enabled = false;
            this->_object = 0;
            ZHTTPD_UNLOCK(this->_mutex);
        }

        inline void destroy()
        {
            ZHTTPD_LOCK(this->_process_mutex);
            this->_enabled = false;
            this->_object = 0;
            ZHTTPD_UNLOCK(this->_process_mutex);
        }

        // From the Selector
        inline void notify(socket_event::Type evt)
        {
            assert(evt >= 0 && evt < socket_event::NB_SOCKET_EVENT);
            this->_events[evt] = true;
        }
    };
}

#endif /* !__SOCKETEVENT_HPP__ */

