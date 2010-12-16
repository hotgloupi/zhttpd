
#include <stdexcept>

#include "parser/ConfigurationParser.hpp"
#include "library/Library.hpp"
#include "socket/SelfConnector.hpp"

#include "RequestManager.hpp"
#include "ConfigurationManager.hpp"
#include "BufferManagerStack.hpp"
#include "TaskManager.hpp"
#include "ServerState.hpp"
#include "Server.hpp"

using namespace zhttpd;

Server::Server() :
    _rcon(0)
{

}

Server::~Server()
{

}

void Server::_loadListeners(Configuration* config)
{
    typedef std::map<api::uint16_t, std::string> ports_t;
    ports_t const& ports = config->getListenPorts();
    ports_t::const_iterator it = ports.begin();
    ports_t::const_iterator ite = ports.end();
    api::size_t errors = 0;
    while (it != ite)
    {
        try
        {
            if (this->_listeners.find(it->first) == this->_listeners.end())
                this->_listeners[it->first] = new ListenerContainer(it->first);
        }
        catch (std::exception& e)
        {
            LOG_ERROR("Cannot listen on " + Logger::toString(it->first) + ": " + e.what());
            this->_listeners[it->first] = 0;
            ++errors;
        }
        ++it;
    }
    if (this->_listeners.size() > 0)
    {
        std::list<api::uint16_t> to_delete;

        listeners_t::iterator it = this->_listeners.begin();
        listeners_t::iterator end = this->_listeners.end();
        for (; it != end; ++it)
        {
            if (ports.find(it->first) == ports.end())
            {
                LOG_INFO("Close port " + Logger::toString(it->first));
                delete it->second;
                to_delete.push_back(it->first);
            }
        }

        std::list<api::uint16_t>::iterator it_del = to_delete.begin();
        for (; it_del != to_delete.end(); ++it_del)
        {
            this->_listeners.erase(*it_del);
        }
    }
    if (errors == ports.size())
    {
        LOG_FATAL("Nothing is listening");
        this->stop();
    }
}

void Server::_stopListeners()
{
    listeners_t::iterator it = this->_listeners.begin();
    listeners_t::iterator ite = this->_listeners.end();
    while (it != ite)
    {
        if (it->second != 0)
        {
            LOG_INFO("Stopping listener on port " + Logger::toString(it->first));
            delete it->second;
            LOG_INFO("Listener Stopped");
        }
        ++it;
    }
    this->_listeners.clear();
}

void Server::_cleanListeners()
{
    listeners_t::iterator it = this->_listeners.begin();
    listeners_t::iterator ite = this->_listeners.end();
    while (it != ite)
    {
        if (it->second != 0)
        {
            LOG_INFO("Delete listener on port " + Logger::toString(it->first));
            ZHTTPD_DELETE(it->second);
        }
        ++it;
    }
}

int Server::run(std::string const& configuration_path)
{
    this->_configuration_path = configuration_path;
    ServerState::getInstance()->setState(ServerState::RUNNING);
    if (this->reload())
    {
        Thread task_manager(TaskManager::getInstance());

        this->_rcon = new Rcon(*this, 10500);
        Thread rcon(this->_rcon);
        LOG_INFO("Server is up");
        rcon.join();
        ZHTTPD_DELETE(this->_rcon);

        LOG_INFO("Server stopping");

        this->_stopListeners();

        LOG_INFO("Wait for TaskManager...");
        task_manager.join();

        this->_cleanListeners();
        TaskManager::delInstance();
        RequestManager::delInstance();
        SessionManager::delInstance();
        BufferManagerStack::delInstance();
        ConfigurationManager::delInstance();
        ServerState::delInstance();
    }
    LOG_INFO("Server is down");
    return 0;
}

void Server::stop()
{
    ServerState::getInstance()->setState(ServerState::STOPPED);
    SelfConnector(10500).run();
}

bool Server::reload()
{
    try
    {
        Configuration *conf = 0;
        parser::ConfigurationParser parser;
        parser.loadFile(this->_configuration_path);
        conf = parser.parse();
        ConfigurationManager::getInstance()->setConfiguration(conf);
        LOG_INFO("New configuration loaded successfully");
        this->_loadListeners(conf);
        return true;
    }
    catch (std::exception& err)
    {
        LOG_ERROR("While loading config: " + err.what());
        return false;
    }
}

