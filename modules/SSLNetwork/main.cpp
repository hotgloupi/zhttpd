
#include "SSLNetworkManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT ZHTTPD::API::IModuleManager* getInstance()
    {
        return new ZHTTPD::MOD::SSLNetworkManager();
    }
}

