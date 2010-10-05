#ifndef __DIRLISTING_HPP__
#define __DIRLISTING_HPP__

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class DirListing : public ZHTTPD::API::IModule
        {
            private:
            public:
                DirListing(API::IModuleManager* manager);
                ~DirListing();
                bool processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer);
        };

    }
}


#endif

