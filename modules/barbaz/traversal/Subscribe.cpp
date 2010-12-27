

#include "BarbazModuleManager.hpp"
#include "Subscribe.hpp"
#include "types/User.hpp"

using namespace traversal;

Subscribe::Subscribe(BarbazModuleManager& manager) : ATraversal<Subscribe>(this), _manager(manager)
{
    this->registerMethod("new_account", &Subscribe::new_account);
}

view::IViewable* Subscribe::index(std::list<std::string>& path, zhttpd::api::IRequest&, zhttpd::mod::PostData&)
{
    return 0;
}

view::IViewable* Subscribe::new_account(zhttpd::api::IRequest&, zhttpd::mod::PostData& data)
{
    types::User* user = new types::User();
    if (data.fields["fullname"].size() > 0)
        user->set_full_name(data.fields["fullname"].front());
    return user;
}
