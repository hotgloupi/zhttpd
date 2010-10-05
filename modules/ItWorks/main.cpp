

#include "ItWorksManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT ZHTTPD::API::IModuleManager* getInstance()
    {
        return new ItWorksManager();
    }
}


