
#include "UserJsonView.hpp"
#include "server/utils/Logger.hpp"


static UserJsonView dummy; //registration at load time

UserJsonView::UserJsonView() : ViewAdapter<IJsonView>(User()) {}


zhttpd::api::IBuffer* UserJsonView::convert(IViewable const& object,
                              zhttpd::api::IBufferManager& manager) const
{
    User const& user = reinterpret_cast<User const&>(object);
    return manager.allocate(
        std::string("{\"id\": ") + zhttpd::Logger::toString(user.id) + "}"
    );
}

