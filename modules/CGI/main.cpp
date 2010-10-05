
# include "CGIManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT ZHTTPD::API::IModuleManager* getInstance()
    {
        return new CGIManager();
    }
}



