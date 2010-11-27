#ifndef __DIRLISTING_HPP__
#define __DIRLISTING_HPP__

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"

namespace zhttpd
{
    namespace mod
    {
        class DirListing : public zhttpd::api::IModule
        {
            private:
            public:
                DirListing(api::IModuleManager* manager);
                ~DirListing();
                bool processRequest(api::event::Type event, api::IRequest* request, api::IBuffer* buffer);
        };

    }
}


#endif

