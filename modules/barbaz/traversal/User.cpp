
# include "BarbazModuleManager.hpp"
# include "traversal/User.hpp"

using namespace traversal;

User::User(BarbazModuleManager& manager) : ATraversal<User>(this), _manager(manager) {}

view::IViewable* User::index(std::list<std::string>& path, zhttpd::api::IRequest& req, zhttpd::mod::PostData& data)
{
    return 0;
}
