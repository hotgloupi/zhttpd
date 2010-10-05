
#include "utils/macros.hpp"
#include "utils/Logger.hpp"
#include "socket/SelfConnector.hpp"

#include "SessionManager.hpp"
#include "Listener.hpp"
#include "ServerState.hpp"

using namespace ZHTTPD;

Listener::Listener(API::uint32_t ip, API::uint16_t port) :
    _ip(ip),
    _port(port),
    _socket(0),
    _session_manager(0),
    _is_running(false),
    _stopped(false)
{
    this->_socket = new Socket(ip, port);
    this->_session_manager = SessionManager::getInstance();
}

Listener::~Listener()
{
    ZHTTPD_DELETE(this->_socket);
}

void Listener::run()
{
    Socket* new_connection = 0;
    ServerState* sv = ServerState::getInstance();
    this->_is_running = true;
    this->_stopped = false;

    LOG_INFO("Listener (on port " + Logger::toString(this->_port) + ") is up");
    while (sv->getState() == ServerState::RUNNING && this->_is_running)
    {
        try {
            new_connection = this->_socket->accept();
            LOG_DEBUG("New connection on port " + Logger::toString(this->_port));
            if (sv->getState() == ServerState::RUNNING && this->_is_running) // stop() can be called
            {
                this->_session_manager->handleNewSession(new_connection, this->_port);
            }
        }
        catch (std::exception& e)
        {
            ZHTTPD_DELETE(new_connection);
            LOG_WARN("Listener error: " + std::string(e.what()));
        }
    }
    ZHTTPD_DELETE(new_connection);
    this->_stopped = true;
    LOG_INFO("Listener (on port " + Logger::toString(this->_port) + ") is down");
}

void Listener::stop()
{
    this->_is_running = false;
    while (this->_stopped == false)
    {
        try {
            SelfConnector(this->_port).run();
        } catch(...) {}
        if (this->_stopped)
            return ;
        Timer::sleep(500);
    }
}

