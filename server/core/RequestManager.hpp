
#ifndef __REQUESTMANAGER_HPP__
# define __REQUESTMANAGER_HPP__

# include <cassert>
# include <map>
# include <boost/asio/io_service.hpp>

# include "utils/Singleton.hpp"
# include "utils/macros.hpp"
# include "socket/Socket.hpp"
# include "socket/SocketPool.hpp"
# include "thread/Mutex.hpp"
# include "thread/Thread.hpp"
# include "configuration/Configuration.hpp"
# include "modules/Parser/ParserManager.hpp"
# include "modules/Builder/BuilderManager.hpp"
# include "modules/PreOutputBuilder/PreOutputBuilderManager.hpp"

# include "BufferManagerStack.hpp"
# include "ConfigurationManager.hpp"
# include "TaskManager.hpp"
# include "Request.hpp"

namespace zhttpd
{


    template<typename Allocator> class RequestManager : public Allocator
    {
    public:
        typedef std::map<Socket*, Request*> requests_t;

    private:
        ConfigurationManager*           _configuration_manager;
        mod::ParserManager*             _parser_manager;
        mod::BuilderManager*            _builder_manager;
        mod::PreOutputBuilderManager*   _preoutputbuilder_manager;
        zhttpd::SocketPool*             _socket_pool;
        Thread*                         _socket_pool_thread;
        boost::asio::io_service         _io_service;

    public:
        _RequestManager() :
            _configuration_manager(0), _parser_manager(0)
        {
            this->_configuration_manager = ConfigurationManager::getInstance();
            this->_parser_manager = new mod::ParserManager();
            this->_builder_manager = new mod::BuilderManager();
            this->_preoutputbuilder_manager = new mod::PreOutputBuilderManager();
            this->_socket_pool = new SocketPool();
            this->_socket_pool_thread = new Thread(this->_socket_pool);
        }

        virtual ~_RequestManager()
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
    };
}

# ifdef ZHTTPD_DEBUG
#  include "utils/SafeAllocator.hpp"
# else
#  include "utils/LockedAllocator.hpp"
# endif
namespace zhttpd
{
# ifdef ZHTTPD_DEBUG
    typedef _RequestManager<SafeAllocator<Request> >  RequestManager;
# else
    typedef _RequestManager<LockedAllocator<Request> >  RequestManager;
# endif
}

#endif /* !__REQUESTMANAGER_HPP__ */

