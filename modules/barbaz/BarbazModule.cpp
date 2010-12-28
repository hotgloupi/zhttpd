
#include <assert.h>

#include "BarbazModuleManager.hpp"
#include "BarbazModule.hpp"
#include "types/User.hpp"
#include "view/IJsonView.hpp"
#include "view/IHTMLView.hpp"
#include "server/utils/String.hpp"
#include "db/ItemList.hpp"
#include "db/ItemCreator.hpp"

BarbazModule::BarbazModule(zhttpd::api::IModuleManager* manager) :
    zhttpd::mod::AbstractProcessingModule<true>(),
    _manager(dynamic_cast<BarbazModuleManager*>(manager))
{
    assert(this->_manager != 0);
}

bool BarbazModule::processOnRequestReady(zhttpd::api::IRequest& request,
                                         zhttpd::mod::PostData& post_data)
{
    BarbazModuleManager::traversals_t const& traversals = this->_manager->getTraversals();
    std::list<std::string> parts = request.getRequestQuery() / '/';
    while (parts.front() == "" || parts.front() == "api")
        parts.pop_front();
    BarbazModuleManager::traversals_t::const_iterator it = traversals.find(parts.front());
    if (it == traversals.end())
    {
        request.raiseError(zhttpd::api::http_code::NOT_FOUND);
        return true;
    }
    traversal::ITraversal* traversal = (*it).second;
    parts.pop_front();
    view::IViewable* res = traversal->traverse(parts, request, post_data);
    if (res != 0)
    {
        view::IViewAdaptor const& adaptor = this->_manager->getViewAdaptor(request);
        types::User u;
        zhttpd::api::IBuffer* buf = adaptor.convert(*res, request.getBufferManager());
        delete res;
        if (buf != 0)
        {
            request.setResponseHeader("Content-Type", adaptor.getContentType());
            request.setResponseHeader("Content-Length", zhttpd::Logger::toString(buf->getSize()));
            request.giveData(buf);
            request.raiseEnd();
            return true;
        }
        else
            LOG_ERROR("Adaptor returns nothing");
    }
    else
        LOG_ERROR("Traversal returns nothing");
    request.raiseError(zhttpd::api::http_code::NOT_FOUND);
    return true;
}
