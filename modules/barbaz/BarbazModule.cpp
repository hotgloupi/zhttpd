
#include <assert.h>

#include "BarbazModuleManager.hpp"
#include "BarbazModule.hpp"
#include "types/User.hpp"
#include "view/IJsonView.hpp"
#include "view/IHTMLView.hpp"
//#include "db/ICursor.hpp"

BarbazModule::BarbazModule(zhttpd::api::IModuleManager* manager) :
    zhttpd::mod::AbstractProcessingModule<true>(),
    _manager(reinterpret_cast<BarbazModuleManager*>(manager))
{
    assert(this->_manager != 0);
}

bool BarbazModule::processOnRequestReady(zhttpd::api::IRequest& request,
                                         zhttpd::mod::PostData* post_data)
{
    User u;
    User::iterator it = u.begin(), end = u.end();
    for (; it != end; ++it)
        std::cout << it.key() << std::endl;
    db::IConnection* conn = this->_manager->getNewDBConnection();
    db::ICursor* curs = conn->cursor();
    curs->execute("SELECT email FROM users WHERE id = ?").bind(1);
    std::cout << curs->fetchone()[0].getString();
    delete curs;
    this->_manager->releaseDBConnection(conn);
    request.setResponseHeader("Content-Type", "text/html");
    request.setResponseCode(zhttpd::api::http_code::OK);
    zhttpd::api::IBuffer* str = IJsonView().convert(u, request.getBufferManager());
    request.setResponseHeader("Content-Length", zhttpd::Logger::toString(str->getSize()));
    request.giveData(str);
    request.raiseEnd();

    return true;
}
