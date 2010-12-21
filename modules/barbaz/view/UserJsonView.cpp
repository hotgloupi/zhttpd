
#include "UserJsonView.hpp"
#include "server/utils/Logger.hpp"
#include "json_spirit.h"

static UserJsonView dummy; //registration at load time

UserJsonView::UserJsonView() : ViewAdaptor<IJsonView>(User()) {}


zhttpd::api::IBuffer* UserJsonView::convert(IViewable const& object,
                                            zhttpd::api::IBufferManager& manager) const
{
    User const& user = dynamic_cast<User const&>(object);
    json_spirit::mObject res;
    res["id"] = static_cast<uint64_t>(user.id);
    res["email"] = user.email;
    res["fullname"] = user.fullname;
    res["password"] = "********";
    res["role"] = user.role;
    res["confirmed"] = user.confirmed;
    res["inscription_date"] = static_cast<uint64_t>(user.inscription_date);
    res["last_login_date"] = static_cast<uint64_t>(user.last_login_date);

    return manager.allocate(json_spirit::write(res));
}

