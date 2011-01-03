
#ifndef __TRAVERSAL_USER_HPP__
# define __TRAVERSAL_USER_HPP__

# include "ATraversal.hpp"

class BarbazModuleManager;

namespace traversal
{
    class User : public ATraversal<User>
    {
    private:
        BarbazModuleManager& _manager;

    public:
        User(BarbazModuleManager& manager);
        view::IViewable* getActions(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
        view::IViewable* getSourceActions(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
        view::IViewable* me(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
    };
}

#endif /* !__USER_HPP__ */

