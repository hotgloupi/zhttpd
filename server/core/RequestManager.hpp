
#ifndef __REQUESTMANAGER_HPP__
# define __REQUESTMANAGER_HPP__

# include <cassert>
# include <map>
# include <boost/asio/io_service.hpp>

# include "utils/macros.hpp"
# include "configuration/Configuration.hpp"
# include "modules/Parser/ParserManager.hpp"
# include "modules/Builder/BuilderManager.hpp"
# include "modules/PreOutputBuilder/PreOutputBuilderManager.hpp"

# include "core/BufferManagerStack.hpp"
# include "core/ConfigurationManager.hpp"
# include "core/TaskManager.hpp"
# include "core/Request.hpp"


namespace zhttpd
{
    template<template <class> class Allocator>
    class RequestManager : private boost::noncopyable
    {
    public:
        typedef std::map<Session::socket_t*, Request*> requests_t;

    private:
        ConfigurationManager*           _configuration_manager;
        mod::ParserManager*             _parser_manager;
        mod::BuilderManager*            _builder_manager;
        mod::PreOutputBuilderManager*   _preoutputbuilder_manager;
        zhttpd::SocketPool*             _socket_pool;
        Thread*                         _socket_pool_thread;
        boost::asio::io_service         _io_service;

    public:
        RequestManager();
        virtual ~RequestManager();
        void handleNewRequest(Session& session);
        void monitorSocketEvent(Request& request);
        void ignoreSocketEvent(Request& request);
        void endRequest(Request* request);
        boost::asio::io_service& getIOService();
    };
}


#endif /* !__REQUESTMANAGER_HPP__ */

