
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
    std::cout << "Search for " << parts.front() << std::endl;
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
            request.giveData(buf);
            request.setResponseHeader("Content-Type", adaptor.getContentType());
            request.raiseEnd();
            return true;
        }
        else
            LOG_ERROR("Adaptor returns nothing");
    }
    else
        LOG_ERROR("Traversal returns nothing");
    return false;
/*
    db::IConnection* conn = this->_manager->getNewDBConnection();
    db::ICursor& curs = conn->cursor();
    curs.execute("SELECT * FROM users");
    db::ItemList list;
    curs.fillItems(list, db::ItemCreator<types::User>());
    std::cout << "Result size : " << list.size() << std::endl;
    for (unsigned int i = 0; i < list.size(); ++i)
    {
        types::User& user = dynamic_cast<types::User&>(list[i]);
        std::cout << "\t* " << i << ": " << user.get_full_name() << std::endl;
    }
    this->_manager->releaseDBConnection(conn);
    request.setResponseHeader("Content-Type", "text/html");
    request.setResponseCode(zhttpd::api::http_code::OK);
    request.giveData(request.getBufferManager().allocate("Hello you"));
    */
    return true;
}
