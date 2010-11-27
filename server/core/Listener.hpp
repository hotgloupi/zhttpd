
#ifndef __LISTENER_HPP__
# define __LISTENER_HPP__

# include "thread/ITask.hpp"
# include "socket/Socket.hpp"

# include "SessionManager.hpp"

namespace zhttpd
{
    /**
     * Cette classe écoute sur une socket et donne la nouvelle socket
     * au RequestFactory
     */
    class Listener : public ITask
    {
    private:
        api::uint32_t _ip;
        api::uint16_t _port;
        Socket* _socket;
        SessionManager* _session_manager;
        volatile bool _is_running;
        volatile bool _stopped;

    public:
        Listener(api::uint32_t ip, api::uint16_t port);
        ~Listener();
        void run();
        void stop();
    };
}

#endif /* !__LISTENER_HPP__ */

