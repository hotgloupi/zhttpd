
#include <assert.h>

#include "BarbazModuleManager.hpp"
#include "BarbazModule.hpp"
#include "types/User.hpp"
#include "view/IJsonView.hpp"
#include "view/IHTMLView.hpp"

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
        User u;
        User::iterator it = u.begin(), end = u.end();
        for (; it != end; ++it)
            std::cout << it.key() << std::endl;

        request->setResponseHeader("Content-Type", "text/html");
        request->setResponseCode(zhttpd::api::http_code::OK);
        zhttpd::api::IBuffer* str = IJsonView().convert(u, request->getBufferManager());
        request->setResponseHeader("Content-Length", zhttpd::Logger::toString(str->getSize()));
        request->giveData(str);
        request->raiseEnd();

        return true;
    }
    return false;
}
