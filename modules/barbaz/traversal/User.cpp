
# include "BarbazModuleManager.hpp"
# include "traversal/User.hpp"

using namespace traversal;

User::User(BarbazModuleManager& manager) : ATraversal<User>(this), _manager(manager)
{
    this->registerMethod("getActions", &User::getActions);
    this->registerMethod("me", &User::me);
}

view::IViewable* User::index(std::list<std::string>& path, zhttpd::api::IRequest& req, zhttpd::mod::PostData& data)
{
    return 0;
}

view::IViewable* User::getActions(zhttpd::api::IRequest&, zhttpd::mod::PostData&)
{
    return 0;
}

view::IViewable* User::me(zhttpd::api::IRequest& req, zhttpd::mod::PostData&)
{
    return this->_manager.getUser(req).release();
}
