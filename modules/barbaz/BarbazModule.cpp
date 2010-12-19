
#include <assert.h>

#include "BarbazModuleManager.hpp"
#include "BarbazModule.hpp"

BarbazModule::BarbazModule(zhttpd::api::IModuleManager* manager) :
    _manager(reinterpret_cast<BarbazModuleManager*>(manager))
{
    assert(this->_manager != 0);
}


bool BarbazModule::processRequest(zhttpd::api::event::Type event,
                                  zhttpd::api::IRequest* request,
                                  zhttpd::api::IBuffer* buffer)
{
    if (event == zhttpd::api::event::ON_END)
    {
        request->setResponseHeader("Content-Type", "text/html");
        request->setResponseCode(zhttpd::api::http_code::OK);
        zhttpd::api::IBuffer* str = request->getBufferManager().allocate(
                "<h1>It works</h1>"
        );
        request->giveData(str);
        request->raiseEnd();
        return true;
    }
    return false;
}
