
#ifndef __USER_HPP__
# define __USER_HPP__

# include "ATraversal.hpp"

namespace traversal
{
    class BarbazModuleManager;

    class User : public ATraversal<User>
    {
    private:
        BarbazModuleManager& _manager;

    public:
        User(BarbazModuleManager& manager);
        virtual view::IViewable* index(std::list<std::string>& path, zhttpd::api::IRequest&, zhttpd::mod::PostData&);
    };
}

#endif /* !__USER_HPP__ */

