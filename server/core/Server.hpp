/**
 * @file Server.hpp
 * @author <londei_a@epitech.net> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <map>

# include "Listener.hpp"
# include "thread/Thread.hpp"
# include "core/Rcon.hpp"
# include "configuration/Configuration.hpp"
# include "utils/macros.hpp"
# include "utils/NonCopyable.hpp"

namespace zhttpd
{
    class ListenerContainer
    {
        Listener* _listener;
        Thread* _thread;
        bool _stopped;

    public:
        ListenerContainer(api::uint16_t port) :
            _listener(0),
            _thread(0),
            _stopped(true)
        {
            this->_listener = new Listener(0, port);
            this->_thread = new Thread(this->_listener);
            this->_stopped = false;
        }

        ~ListenerContainer()
        {
            this->stop();
            ZHTTPD_DELETE(this->_listener);
            ZHTTPD_DELETE(this->_thread);
        }

        void stop()
        {
            if (this->_stopped)
                return;
            this->_listener->stop();
            this->_thread->join();
            this->_stopped = true;
        }
    };

    class Server : private NonCopyable
    {
        typedef std::map<api::uint16_t, ListenerContainer*> listeners_t;

    private:
        listeners_t _listeners;
        Rcon* _rcon;
        std::string _configuration_path;

   public:
        Server();
        ~Server();
        int run(std::string const& configuration_path);
        void stop();
        bool reload();

    private:
        void _cleanup();
        void _loadListeners(Configuration* config);
        void _stopListeners();
        void _cleanListeners();
    };
}

#endif /* !__SERVER_HPP__ */

