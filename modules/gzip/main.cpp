#include "GzipManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT zhttpd::api::IModuleManager* getInstance()
    {
        return new GzipManager();
    }
}

