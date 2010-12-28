
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
        virtual view::IViewable* index(std::list<std::string>&, zhttpd::api::IRequest&, zhttpd::mod::PostData&)
        {
            return 0;
        }
        view::IViewable* login(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
        view::IViewable* loginWithHash(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
    };
}

#endif /* !__LOGIN_HPP__ */

