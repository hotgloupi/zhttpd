
#include "utils/Logger.hpp"
#include "modules/ModError/ModErrorManager.hpp"
#include "modules/DirListing/DirListingManager.hpp"

#include "Configuration.hpp"
#include "ModuleConfiguration.hpp"

using namespace ZHTTPD;

ModuleConfiguration::ModuleConfiguration(std::string const& name, bool enabled, Configuration& config) :
    _name(name),
    _options(),
    _manager(0),
    _merged(false),
    _loaded(false),
    _enabled(enabled),
    _config(config)
{
}

ModuleConfiguration::~ModuleConfiguration()
{
    ZHTTPD_DELETE(this->_manager);
}

void ModuleConfiguration::addConfigurationEntry(std::string const& key, std::string const& value)
{
    this->_options[key] = value;
}

std::string const& ModuleConfiguration::getConfigurationEntry(std::string const& key)
{
    return this->_options[key];
}

std::string const& ModuleConfiguration::getName() const
{
    return this->_name;
}

bool ModuleConfiguration::isEnabled() const
{
    return this->_enabled;
}

ZHTTPD::API::IModuleManager* ModuleConfiguration::getModuleManager()
{
    if (this->_loaded == false && this->_enabled)
    {
        this->_manager = this->_config.getModuleManager(this->_name);
        if (this->_manager != 0)
        {
            this->_setConfiguration(this->_manager);
            this->_loaded = true;
            LOG_DEBUG("creating " + this->_manager->getName() + " manager ok");
        }
        else
        {
            LOG_WARN("Cannot find any ModuleManager named '" + this->_name + "'");
            this->_enabled = false;
        }
    }
    return this->_manager;
}

void ModuleConfiguration::_setConfiguration(ZHTTPD::API::IModuleManager* manager)
{
    std::map<std::string, std::string>::iterator it = this->_options.begin();
    std::map<std::string, std::string>::iterator ite = this->_options.end();
    while (it != ite)
    {
        manager->addConfigurationEntry(it->first, it->second);
        LOG_DEBUG("Adding option " + it->first + "=" + it->second + " to " + this->_name);
        ++it;
    }
}

void ModuleConfiguration::merge(ModuleConfiguration const& parent)
{
    LOG_DEBUG("merging module " + this->getName());
    options_t const& parent_options = parent.getOptions();
    options_t::const_iterator it = parent_options.begin();
    options_t::const_iterator end = parent_options.end();
    for (; it != end; ++it)
    {
        if (this->_options.find(it->first) == this->_options.end())
            this->_options[it->first] = it->second;
    }
}

ModuleConfiguration::options_t const& ModuleConfiguration::getOptions() const
{
    return this->_options;
}

bool ModuleConfiguration::isMerged() const
{
    return this->_merged;
}

void ModuleConfiguration::setMerged(bool b)
{
    this->_merged = b;
}

