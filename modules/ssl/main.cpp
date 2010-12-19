
#include "SSLNetworkManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT zhttpd::api::IModuleManager* getInstance()
    {
        return new zhttpd::mod::SSLNetworkManager();
    }
}

