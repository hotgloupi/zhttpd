
# include "BarbazModuleManager.hpp"
# include "traversal/User.hpp"
# include "db/ItemList.hpp"
# include "types/UserAction.hpp"

using namespace traversal;

User::User(BarbazModuleManager& manager) : ATraversal<User>(this), _manager(manager)
{
    this->registerMethod("actions", &User::getActions);
    this->registerMethod("me", &User::me);
}

view::IViewable* User::index(std::list<std::string>& path, zhttpd::api::IRequest& req, zhttpd::mod::PostData& data)
{
    return 0;
}

view::IViewable* User::getActions(zhttpd::api::IRequest& req, zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user(this->_manager.getUser(req));
    if (user.get() == 0)
        req.raiseError(zhttpd::api::http_code::FORBIDDEN);
    db::ItemList* res = new db::ItemList;
    if (user->get_role() == "manager" || user->get_role() == "admin")
    {
        res->push_back(*(new types::UserAction("Manage users", "manage_users.frag.html")));
        res->push_back(*(new types::UserAction("Manage sources", "manage_sources.frag.html")));
    }
    return res;
}

view::IViewable* User::me(zhttpd::api::IRequest& req, zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user(this->_manager.getUser(req));
    if (user.get() == 0)
        req.raiseError(zhttpd::api::http_code::FORBIDDEN);
    return user.release();
}
