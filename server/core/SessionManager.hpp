
#ifndef __SESSIONMANAGER_HPP__
# define __SESSIONMANAGER_HPP__

# include <boost/noncopyable.hpp>

# include "Session.hpp"
# include "Request.hpp"
# include "BufferManagerStack.hpp"
# include "RequestManager.hpp"

namespace zhttpd
{

# ifndef ZHTTPD_SESSION_ALLOCATOR
#  ifdef ZHTTPD_DEBUG
#   include "utils/SafeAllocator.hpp"
#   define ZHTTPD_SESSION_ALLOCATOR SafeAllocator
#  else
#   include "utils/LockedAllocator.hpp"
#   define ZHTTPD_SESSION_ALLOCATOR LockedAllocator
#  endif
# endif

    template< template <class> class Allocator = ZHTTPD_SESSION_ALLOCATOR >
    class SessionManager :
        public Allocator<Session>,
        private boost::noncopyable
    {
    private:
        RequestManager _request_manager;

    public:
        SessionManager() : _request_manager()
        {
        }

        virtual ~SessionManager()
        {
        }

    public:
        void onNewSession(std::auto_ptr<Listener::socket_t>& socket, api::uint16_t port)
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

