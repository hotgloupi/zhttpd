
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

    template< template <class> class SessionAllocator = ZHTTPD_SESSION_ALLOCATOR >
    class SessionManager :
        public SessionAllocator<Session>,
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
        void onNewSession(Session& new_session, api::uint16_t)
        {
            this->_request_manager.handleNewRequest(new_session);
        }

        void endSession(Session& session)
        {
            SessionAllocator<Session>::release(&session);
        }
        boost::asio::io_service& getIOService()
        {
            return this->_request_manager.getIOService();
        }
    };
}


#endif /* !__SESSIONMANAGER_HPP__ */

