
#include <boost/bind.hpp>

#include <stdexcept>


#include "parser/ConfigurationParser.hpp"
#include "library/Library.hpp"
#include "socket/SelfConnector.hpp"
# include "thread/Thread.hpp"

#include "core/Server.hpp"
#include "core/Session.hpp"
#include "core/SessionManager.hpp"
#include "core/Listener.hpp"
#include "core/Rcon.hpp"
#include "configuration/Configuration.hpp"
#include "utils/macros.hpp"

#include "core/SessionManager.hpp"
#include "RequestManager.hpp"
#include "ConfigurationManager.hpp"
#include "BufferManagerStack.hpp"
#include "TaskManager.hpp"
#include "ServerState.hpp"

using namespace zhttpd;

Server::Server() :
    _session_manager(new SessionManager<>()),
    _io_service(new boost::asio::io_service()),
    _listener(0),
    _rcon(0),
    _configuration_path()
{
}

Server::~Server()
{
    ZHTTPD_DELETE(this->_io_service);
    ZHTTPD_DELETE(this->_session_manager);
}

void Server::_loadListener(Configuration* config)
{
    typedef std::map<api::uint16_t, std::string> ports_t;
    ports_t const& ports = config->getListenPorts();
    ports_t::const_iterator it = ports.begin(), end = ports.end();
    if (this->_listener != 0)
    {
        this->_listener->stop();
        ZHTTPD_DELETE(this->_listener);
    }
    this->_listener = new Listener(
        boost::bind(&SessionManager<>::onNewSession, this->_session_manager, _1, _2),
        boost::bind(&Server::_getIOService, this)
    );
    for (; it != end; ++it)
    {
        this->_listener->bind(
            boost::asio::ip::address::from_string(it->second),
            it->first
        );
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

        if (this->_listener != 0)
        {
            this->_listener->stop();
            ZHTTPD_DELETE(this->_listener);
        }

        LOG_INFO("Wait for TaskManager...");
        task_manager.join();

        TaskManager::delInstance();
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
        this->_loadListener(conf);
        return true;
    }
    catch (std::exception& err)
    {
        LOG_ERROR("While loading config: " + err.what());
        return false;
    }
}

boost::asio::io_service& Server::_getIOService()
{
    return this->_session_manager->getIOService();
}
