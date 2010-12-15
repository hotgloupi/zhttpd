#ifndef __RCON_HPP__
# define __RCON_HPP__

# include <string>

# include "thread/ITask.hpp"
# include "socket/Socket.hpp"

namespace zhttpd
{
    class Server;

    class Rcon : public ITask
    {
        private:
            enum
            {
                MAX_COMMAND_LENGTH = 1024,
                READ_SIZE = 4096
            };
            bool _rconSession(Socket* socket);
            void _readCommands(Socket* socket);
            bool _executeCommand(Socket* socket, std::string const& command);
            void _sendString(Socket* socket, std::string const& line);
            Server& _server;
            Socket* _socket;
            std::string _buffer;
        public:
            Rcon(Server& server, api::uint16_t port);
            ~Rcon();
            virtual void run();
    };
}

#endif

