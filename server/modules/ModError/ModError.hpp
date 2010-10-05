
#ifndef __MODERROR_HPP__
# define __MODERROR_HPP__

# include "utils/Logger.hpp"
# include "core/Request.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class ModErrorManager;

        class ModError : public ZHTTPD::API::IModule
        {
        public:
            ModError(ZHTTPD::API::IModuleManager*) {}
            bool processRequest(ZHTTPD::API::EVENT::Type event,
                                ZHTTPD::API::IRequest* request,
                                ZHTTPD::API::IBuffer* buffer)
            {
                if (buffer != 0)
                    request->getBufferManager().release(buffer);
                if (event == ZHTTPD::API::EVENT::ON_END)
                {
                    request->setResponseHeader("Content-Type", "text/html");
                    ZHTTPD::API::IBuffer* b = request->getBufferManager().allocate(
                        "<h1>" +
                        Logger::toString(request->getResponseCode()) + " " +
                        request->getResponseMessage() +
                        "</h1>"
                    );
                    request->setResponseHeader("Content-Length", Logger::toString(b->getSize()));
                    request->giveData(b);
                    request->raiseEnd();
                    return true;
                }
                return true;
            }
        };
    }
}

#endif /* !__MODERROR_HPP__ */

