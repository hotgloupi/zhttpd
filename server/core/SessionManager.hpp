
#ifndef __SESSIONMANAGER_HPP__
# define __SESSIONMANAGER_HPP__

# include "utils/Singleton.hpp"

# include "Session.hpp"
# include "Request.hpp"
# include "BufferManagerStack.hpp"
# include "RequestManager.hpp"

namespace ZHTTPD
{
    template<typename SessionAllocator>
    class _SessionManager : public Singleton< _SessionManager<SessionAllocator> >, public SessionAllocator
    {
        friend class Singleton<_SessionManager>;

    private:
        RequestManager* _request_manager;

    protected:
        _SessionManager()
        {
            this->_request_manager = RequestManager::getInstance();
        }

        virtual ~_SessionManager()
        {
        }

    public:
        void handleNewSession(Socket* socket, API::uint16_t port)
        {
            Session* session = SessionAllocator::allocate(socket, port);
            this->_request_manager->handleNewRequest(*session);
        }

        void endSession(Session& session)
        {
            SessionAllocator::release(&session);
        }
    };
}

# ifdef ZHTTPD_DEBUG
#  include "utils/SafeAllocator.hpp"
# else
#  include "utils/LockedAllocator.hpp"
# endif
namespace ZHTTPD
{
# ifdef ZHTTPD_DEBUG
    typedef _SessionManager<SafeAllocator<Session> > SessionManager;
# else
    typedef _SessionManager<LockedAllocator<Session> > SessionManager;
# endif
}

#endif /* !__SESSIONMANAGER_HPP__ */

