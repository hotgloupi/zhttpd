
#include "utils/Logger.hpp"
#include "utils/macros.hpp"
#include "core/Server.hpp"
#include "utils/StatsManager.hpp"

#include "Rcon.hpp"

using namespace ZHTTPD;

Rcon::Rcon(Server& server, API::uint16_t port) :
    _server(server), _socket(new Socket(0x7f000001, port))
{
}

Rcon::~Rcon()
{
    ZHTTPD_DELETE(this->_socket);
}

void Rcon::_readCommands(Socket* socket)
{
    static char buf[READ_SIZE];
    while (true)
    {
        unsigned int bytes = socket->read(buf, READ_SIZE);
        for (unsigned int i = 0; i < bytes; ++i)
            if (buf[i] == '\n')
            {
                if (this->_executeCommand(socket, this->_buffer))
                    return;
                this->_buffer = "";
            }
            else if (this->_buffer.size() > MAX_COMMAND_LENGTH)
                this->_buffer = "";
            else
                this->_buffer.push_back(buf[i]);
    }
}

void Rcon::_sendString(Socket* socket, std::string const& line)
{
    API::size_t bytes = 0;
    while (bytes < line.size())
        bytes += socket->write(line.c_str() + bytes, line.size() - bytes);
}

bool Rcon::_executeCommand(Socket* socket, std::string const& command)
{
    LOG_DEBUG("Received command: \"" + command + "\".");
    if (command == "stop")
    {
        LOG_INFO("Stopping server...");
        this->_server.stop();
        ZHTTPD_DELETE(socket);
        return true;
    }
    else if (command == "reload")
    {
        if (this->_server.reload())
            this->_sendString(socket, "Server reloaded.\n");
        else
            this->_sendString(socket, "Failed to reload server.\n");
    }
    else if (command == "get")
    {
        std::stringstream ss;
        ss << StatsManager::getInstance()->getUptime() << " ";
        ss << StatsManager::getInstance()->getSentBytes() << " ";
        ss << StatsManager::getInstance()->getReceivedBytes() << " ";
        ss << StatsManager::getInstance()->getRequests() << " ";
        ss << StatsManager::getInstance()->getBuffers() << " ";
        ss << StatsManager::getInstance()->getMemory() << " ";
        ss << "\n";
        this->_sendString(socket, ss.str());
    }
    return false;
}

bool Rcon::_rconSession(Socket* socket)
{
    bool status = false;
    LOG_INFO("Remote configuration activated!");
    try
    {
        StatsManager::getInstance()->getSentBytes(); // reset 0
        StatsManager::getInstance()->getReceivedBytes(); // reset 0
        StatsManager::getInstance()->getRequests(); // reset 0
        this->_readCommands(socket);
    }
    catch (std::exception& e)
    {
        status = true;
    }
    LOG_INFO("Remote configuration stopped!");
    return status;
}

void Rcon::run()
{
    ServerState* sv = ServerState::getInstance();
    Socket* client;
    while (sv->getState() == ServerState::RUNNING)
    {
        // TODO add accept timeout and use it
        client = this->_socket->accept();
        if (sv->getState() == ServerState::RUNNING)
        {
            if (this->_rconSession(client))
                ZHTTPD_DELETE(client);
        }
        else
            ZHTTPD_DELETE(client);
    }
}

