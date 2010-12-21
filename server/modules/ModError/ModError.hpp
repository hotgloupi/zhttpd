
#ifndef __MODERROR_HPP__
# define __MODERROR_HPP__

# include "utils/Logger.hpp"
# include "core/Request.hpp"

namespace zhttpd
{
    namespace mod
    {
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
                    request->setResponseHeader("Connection", "close");
                    request->setResponseHeader("Content-Type", "text/html");
                    //request->setResponseHeader("Date", "Tue, 21 Dec 2010 18:25:42 GMT");

                    std::string msg = Logger::toString(request->getResponseCode())  + " - " + request->getResponseMessage();
                    zhttpd::api::IBuffer* b = request->getBufferManager().allocate(
                        "<html><head><title>" + msg + "</title></head><body><h1>" + msg + "</h1></body></html>\r\n"
                    );
                    //request->setResponseHeader("Content-Length", Logger::toString(b->getSize()));
                    if (b->getSize() < 512)
                    {
                        // Chrome and IE won't show the page if it's less than 512 bytes
                        request->giveData(request->getBufferManager().allocate(
"                                                                                                                      "
"                                                                                                                      "
"                                                                                                                      "
"                                                                                                                      "
"                                                                                                                      "
                        ));
                    }
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

