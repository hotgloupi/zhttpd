
#include "server/utils/Logger.hpp"
#include "types/Response.hpp"
#include "ResponseJsonView.hpp"
#include "json_spirit.h"

using namespace view;

static ResponseJsonView dummy; //registration at load time

ResponseJsonView::ResponseJsonView() : ViewAdaptor<IJsonView>(types::Response()) {}

zhttpd::api::IBuffer* ResponseJsonView::convert(IViewable const& object,
                                              zhttpd::api::IBufferManager& manager) const
{
    types::Response const& response = dynamic_cast<types::Response const&>(object);
    types::Response::fields_t const& fields = response.getFields();
    types::Response::fields_t const& details = response.getDetails();
    json_spirit::Object res;
    types::Response::fields_t::const_iterator it = fields.begin(), end = fields.end();
    for (; it != end; ++it)
        res.push_back(json_spirit::Pair((*it).first, (*it).second));

    if (details.size() > 0)
    {
        json_spirit::Object details_res;
        types::Response::fields_t::const_iterator it = details.begin(), end = details.end();
        for (; it != end; ++it)
            details_res.push_back(json_spirit::Pair((*it).first, (*it).second));
        res.push_back(json_spirit::Pair("details", details_res));
    }
    return manager.allocate(json_spirit::write(res));
}

