
#ifndef __SESSIONMANAGER_HPP__
# define __SESSIONMANAGER_HPP__

# include <boost/noncopyable.hpp>

# include "RequestManager.hpp"
# include "Session.hpp"
# include "Request.hpp"
# include "BufferManagerStack.hpp"

namespace zhttpd
{
    template<template <class> class Allocator>
    class SessionManager : private boost::noncopyable
    {
    private:
        RequestManager<Allocator> _request_manager;

    public:
        SessionManager() : _request_manager()
        {
        }

        virtual ~SessionManager()
        {
        }

    public:
        void onNewSession(boost::shared_ptr<Session::socket_t>& socket, api::uint16_t port)
        {

            Allocator<Session>::allocate(socket, port);
            this->_request_manager.handleNewRequest();
        }

        void endSession(Session& session)
        {
            Allocator<Session>::release(&session);
        }

        boost::asio::io_service& getIOService()
        {
            return this->_request_manager.getIOService();
        }
    };
}


#endif /* !__SESSIONMANAGER_HPP__ */

