
#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "utils/Logger.hpp"

#include "ItWorks.hpp"

ItWorks::ItWorks() : _count(0) {}

bool ItWorks::processRequest(ZHTTPD::API::EVENT::Type event, ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer*)
{
    if (event == ZHTTPD::API::EVENT::ON_END || event == ZHTTPD::API::EVENT::ON_IDLE)
    {
        if (this->_count == 0)
        {
            request->setResponseHeader("Content-Type", "text/html");
            request->setResponseCode(ZHTTPD::API::HTTP_CODE::OK);
        }
        ZHTTPD::API::IBuffer* str = request->getBufferManager().allocate(
                "<h1>It works: " +
                ZHTTPD::Logger::toString(this->_count) +
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

