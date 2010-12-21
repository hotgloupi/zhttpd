
#include <iostream>

#include "BarbazModuleManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT zhttpd::api::IModuleManager* getInstance()
    {
        return new BarbazModuleManager();
    }
}

