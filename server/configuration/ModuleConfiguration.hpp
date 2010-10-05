
#ifndef __MODULECONFIGURATION_HPP__
# define __MODULECONFIGURATION_HPP__

# include <map>
# include <string>

# include "api/IModuleManager.hpp"

namespace ZHTTPD
{
    class Configuration;

    class ModuleConfiguration
    {
        typedef std::map<std::string, std::string> options_t;

    private:
        std::string                         _name;
        options_t                           _options;
        API::IModuleManager*                _manager;
        bool                                _merged;
        bool                                _loaded;
        bool                                _enabled;
        Configuration&                      _config;

        void _setConfiguration(ZHTTPD::API::IModuleManager* manager);

    public:
        ModuleConfiguration(std::string const& name, bool enabled, Configuration& config);
        virtual ~ModuleConfiguration();

        void addConfigurationEntry(std::string const& key, std::string const& value);
        std::string const& getConfigurationEntry(std::string const& key);
        std::string const& getName() const;
        API::IModuleManager* getModuleManager();
        options_t const& getOptions() const;

        void merge(ModuleConfiguration const& parent);
        bool isMerged() const;
        void setMerged(bool b = true);

        bool isEnabled() const;
    };
}

#endif // MODULE_HPP__

