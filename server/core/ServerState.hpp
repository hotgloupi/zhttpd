
#ifndef __SERVERSTATE_HPP__
# define __SERVERSTATE_HPP__

# include "utils/Singleton.hpp"

namespace zhttpd
{
    class ServerState : public Singleton<ServerState>
    {
        friend class Singleton<ServerState>;
    public:
        enum State
        {
            RUNNING = 1,
            STOPPED = 2,
        };

    private:
        volatile State _state;

    private:
        ServerState() : _state(STOPPED) {}

    public:
        void setState(State s)
        {
            // XXX mutex, atomic ?
            this->_state = s;
        }

        State getState()
        {
            return this->_state;
        }

    };
}

#endif /* !__SERVERSTATE_HPP__ */

