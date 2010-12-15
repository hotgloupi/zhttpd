
#ifndef __CONFIGURATION_HPP__
# define __CONFIGURATION_HPP__

# include <map>
# include <list>
# include <string>

# include "utils/NonCopyable.hpp"

# include "VHost.hpp"
# include "ModuleConfiguration.hpp"
# include "ModuleManagerFactory.hpp"

namespace zhttpd
{
    class Configuration : public NonCopyable
    {
    public:
        typedef std::map<api::category::Type, std::list<std::string> > available_modules_t;
        typedef std::map<std::string, ModuleConfiguration*> modules_configuration_t;
        typedef std::map<api::uint16_t, std::string> io_modules_t;

    private:
        modules_configuration_t         _modules_configuration;
        ModuleManagerFactory            _factory;
        std::list<std::string>          _index_files;
        VHost*                          _vhost;
        io_modules_t                    _ports;

    public:
        Configuration();
        ~Configuration();
        VHost&                          getVHost() const;
        void                            addModuleConfiguration(ModuleConfiguration& module_configuration);
        modules_configuration_t const&  getModulesConfiguration() const;
        ModuleConfiguration&            getModuleConfiguration(std::string const& name);
        api::IModuleManager*            getModuleManager(std::string const& name);
        available_modules_t const&      getAvailableModules() const;
        io_modules_t const&             getListenPorts() const;
        void                            addListenPort(api::uint16_t port, std::string const& io_module);
        void                            setVHost(VHost& vhost);
        api::IModuleManager&            getInputOutputModuleManager(api::uint16_t port);
        void                            addModuleDirectory(std::string const& path);
        void                            addIndexFile(std::string const& filename);
        std::list<std::string> const&   getIndexFiles() const;
    };
}

#endif // __CONFIGURAION_HPP__

