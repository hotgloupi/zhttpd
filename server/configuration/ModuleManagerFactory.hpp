
#ifndef __MODULEMANAGERFACTORY_HPP__
# define __MODULEMANAGERFACTORY_HPP__

# include <map>
# include <string>

# include "api/IModuleManager.hpp"
# include "library/Library.hpp"
# include "utils/NonCopyable.hpp"

namespace zhttpd
{
    class ModuleManagerFactory : public NonCopyable
    {
        typedef api::IModuleManager* (ModuleManagerFactory::*creator_t)(std::string const&) const;
        typedef api::IModuleManager* (*lib_handler_t)();
        typedef std::map< api::category::Type, std::list<std::string> > available_modules_t;

    private:
        std::map<std::string, Library*>     _libraries;
        std::map<std::string, creator_t>    _builders;
        available_modules_t                 _available_modules;

    public:
        ModuleManagerFactory();
        ~ModuleManagerFactory();
        void findModules(std::string const& modules_directory);
        api::IModuleManager* getModuleManager(std::string const& name) const;
        available_modules_t const& getAvailableModules() const;

    private:
        api::IModuleManager* _createNetworkModuleManager(std::string const& name) const;
        api::IModuleManager* _createFileReaderModuleManager(std::string const& name) const;
        api::IModuleManager* _createModErrorModuleManager(std::string const& name) const;
        api::IModuleManager* _createDirListingModuleManager(std::string const&) const;
        api::IModuleManager* _createFromLibrary(std::string const& name) const;
        bool _isDynamicLibrary(std::string const& filename) const;
    };
}

#endif /* !__MODULEMANAGERFACTORY_HPP__ */

