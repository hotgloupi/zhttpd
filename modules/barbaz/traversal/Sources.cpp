
# include "BarbazModuleManager.hpp"
# include "Sources.hpp"
# include "broker/Sources.hpp"

using namespace traversal;

Sources::Sources(BarbazModuleManager& manager) : ATraversal<Sources>(this), _manager(manager)
{
    this->registerMethod("mine", &Sources::mine);
}

view::IViewable* Sources::mine(zhttpd::api::IRequest& req, zhttpd::mod::PostData&)
{
    std::auto_ptr<types::User> user = this->_manager.getUser(req);
    if (user.get() == 0)
        return 0;
    return broker::Sources::getUserSources(*this->_manager.getNewDBConnection(), user->get_id());
}
