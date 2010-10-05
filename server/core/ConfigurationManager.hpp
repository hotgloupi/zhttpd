
#ifndef __CONFIGURATIONMANAGER_HPP__
# define __CONFIGURATIONMANAGER_HPP__

# include <cassert>

# include "utils/Singleton.hpp"
# include "utils/SmartPtr.hpp"
# include "utils/macros.hpp"
# include "thread/Mutex.hpp"

# include "configuration/Configuration.hpp"

namespace ZHTTPD
{
    class ConfigurationManager : public Singleton<ConfigurationManager>
    {
        friend class Singleton<ConfigurationManager>;
    private:
        SmartPtr<Configuration> _configuration;
        Mutex _mutex;

    private:
        ConfigurationManager() : _configuration(0) {}

    public:
        void setConfiguration(Configuration* configuration)
        {
            ZHTTPD_LOCK(this->_mutex);
            this->_configuration = SmartPtr<Configuration>(configuration);
            ZHTTPD_UNLOCK(this->_mutex);
        }

        SmartPtr<Configuration> getConfiguration()
        {
            ZHTTPD_LOCK(this->_mutex);
            assert(this->_configuration.get() != 0);
            SmartPtr<Configuration> config = this->_configuration;
            ZHTTPD_UNLOCK(this->_mutex);
            return config;
        }
    };
}

#endif /* !__CONFIGURATIONMANAGER_HPP__ */

