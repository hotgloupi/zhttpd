
#ifndef __LOGIN_HPP__
# define __LOGIN_HPP__

# include "ATraversal.hpp"

class BarbazModuleManager;

namespace traversal
{
    class Login : public ATraversal<Login>
    {
        BarbazModuleManager& _manager;

    public:
        Login(BarbazModuleManager& manager);
        virtual view::IViewable* index(std::list<std::string>&, zhttpd::api::IRequest& request, zhttpd::mod::PostData&)
        {
            if (this->_manager.getUser(request).get() != 0)
            {
                request.setResponseHeader("Location", "/app.html");
                request.setResponseCode(zhttpd::api::http_code::FOUND);
            }
            else
            {
                request.raiseError(zhttpd::api::http_code::NOT_FOUND);
            }
            return 0;
        }
        view::IViewable* login(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
        view::IViewable* logout(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
        view::IViewable* loginWithHash(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
    };
}

#endif /* !__LOGIN_HPP__ */

