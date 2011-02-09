
#include "RequestManager.hpp"

template<typename Allocator>
RequestManage::RequestManager() :
    _configuration_manager(0), _parser_manager(0)
{
    this->_configuration_manager = ConfigurationManager::getInstance();
    this->_parser_manager = new mod::ParserManager();
    this->_builder_manager = new mod::BuilderManager();
    this->_preoutputbuilder_manager = new mod::PreOutputBuilderManager();
    this->_socket_pool = new SocketPool();
    this->_socket_pool_thread = new Thread(this->_socket_pool);
}

virtual RequestManager()
{
    ZHTTPD_DELETE(this->_parser_manager);
    ZHTTPD_DELETE(this->_builder_manager);
    ZHTTPD_DELETE(this->_preoutputbuilder_manager);
    LOG_INFO("Stopping RequestManager SocketPool...");
    this->_socket_pool->stop();
    this->_socket_pool_thread->join();
    LOG_INFO("SocketPool stopped");
    ZHTTPD_DELETE(this->_socket_pool);
    ZHTTPD_DELETE(this->_socket_pool_thread);
}

public:
void handleNewRequest(Session& session)
{
    SmartPtr<Configuration> config = this->_configuration_manager->getConfiguration();
    Request* request = Allocator::allocate(
            *(BufferManagerStack::getInstance()->pop()),
            session,
            config
            );
    LOG_DEBUG("CREATING REQUEST "+Logger::toString(request));
    api::IModuleManager& io = config->getInputOutputModuleManager(session.getPort());
    request->append(io, *io.getInstance(false));
    request->append(*this->_parser_manager, *this->_parser_manager->getInstance());
    request->append(*this->_builder_manager, *this->_builder_manager->getInstance());
    request->append(*this->_preoutputbuilder_manager, *this->_preoutputbuilder_manager->getInstance());
    request->append(io, *io.getInstance(true));
    request->getRequestTasks().needRead();
    TaskManager::getInstance()->startRequest(*request);
}

void monitorSocketEvent(Request& request)
{
    this->_socket_pool->registerHandler(
            request.getSession().getSocket(),
            request.getRequestTasks()
            );
}

void ignoreSocketEvent(Request& request)
{
    this->_socket_pool->unregisterHandler(
            request.getSession().getSocket()
            );
}

void endRequest(Request* request)
{
    LOG_DEBUG("ENDING REQUEST "+Logger::toString(request));
    assert(!request->isQueued() && "must not be queued");
    api::IBufferManager* buffer_manager = &request->getBufferManager();
#ifdef ZHTTPD_DEBUG
    SafeBufferManager* safe_buffer_manager = dynamic_cast<SafeBufferManager*>(buffer_manager);
    assert(safe_buffer_manager != 0);
    buffer_manager = &safe_buffer_manager->getBufferManager();
#endif
    BufferManagerStack::getInstance()->push(buffer_manager);
    //            if (request->getServerSession().getServerSocketPtr() != 0)
    //                this->_socket_pool->destroyHandler(request->getSession().getSocket());
    Allocator::release(request);
}

boost::asio::io_service& getIOService()
{
    return this->_io_service;
}
