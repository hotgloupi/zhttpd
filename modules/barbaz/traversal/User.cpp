
# include "BarbazModuleManager.hpp"
# include "traversal/User.hpp"
# include "db/ItemList.hpp"
# include "types/UserAction.hpp"

using namespace traversal;


User::User(BarbazModuleManager& manager) : ATraversal<User>(this), _manager(manager)
{
    this->registerMethod("actions", &User::getActions);
    this->registerMethod("source_actions", &User::getSourceActions);
    this->registerMethod("me", &User::me);
}


view::IViewable* User::getActions(zhttpd::api::IRequest& req, zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user(this->_manager.getUser(req));
    if (user.get() == 0)
        req.raiseError(zhttpd::api::http_code::FORBIDDEN);
    db::ItemList* res = new db::ItemList;
    if (user->get_role() == "manager" || user->get_role() == "admin")
    {
        res->push_back(types::UserAction("Manage users", "manage_users.frag.html"));
        res->push_back(types::UserAction("Manage sources", "manage_sources.frag.html"));
    }
    return res;
}

view::IViewable* User::getSourceActions(zhttpd::api::IRequest& req,
                                  zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user(this->_manager.getUser(req));
    if (user.get() == 0)
        req.raiseError(zhttpd::api::http_code::FORBIDDEN);
    db::ItemList* res = new db::ItemList;

    if (user->get_role() == "admin")
    {
        res->push_back(*(new types::UserAction("Add local source",
                                               "add_local_source.frag.html")));
    }
    res->push_back(*(new types::UserAction("Add source",
                                           "add_user_source.frag.html")));
    return res;
}

view::IViewable* User::me(zhttpd::api::IRequest& req, zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user(this->_manager.getUser(req));
    if (user.get() == 0)
        req.raiseError(zhttpd::api::http_code::FORBIDDEN);
    return user.release();
}
