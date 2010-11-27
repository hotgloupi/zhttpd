
#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "utils/Logger.hpp"

#include "ItWorks.hpp"

ItWorks::ItWorks() : _count(0) {}

bool ItWorks::processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer*)
{
    if (event == zhttpd::api::event::ON_END || event == zhttpd::api::event::ON_IDLE)
    {
        if (this->_count == 0)
        {
            request->setResponseHeader("Content-Type", "text/html");
            request->setResponseCode(zhttpd::api::http_code::OK);
        }
        zhttpd::api::IBuffer* str = request->getBufferManager().allocate(
                "<h1>It works: " +
                zhttpd::Logger::toString(this->_count) +
                "</h1>"
        );
        request->giveData(str);
        if (this->_count < 0)
            request->callLater(1000);
        else
            request->raiseEnd();
        this->_count++;
        return true;
    }
    return false;
}

