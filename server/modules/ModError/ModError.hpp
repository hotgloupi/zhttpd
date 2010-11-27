
#ifndef __MODERROR_HPP__
# define __MODERROR_HPP__

# include "utils/Logger.hpp"
# include "core/Request.hpp"

namespace zhttpd
{
    namespace mod
    {
        class ModErrorManager;

        class ModError : public zhttpd::api::IModule
        {
        public:
            ModError(zhttpd::api::IModuleManager*) {}
            bool processRequest(zhttpd::api::event::Type event,
                                zhttpd::api::IRequest* request,
                                zhttpd::api::IBuffer* buffer)
            {
                if (buffer != 0)
                    request->getBufferManager().release(buffer);
                if (event == zhttpd::api::event::ON_END)
                {
                    request->setResponseHeader("Content-Type", "text/html");
                    zhttpd::api::IBuffer* b = request->getBufferManager().allocate(
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

