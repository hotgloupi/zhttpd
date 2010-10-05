
#include <stdexcept>


#include "modules/Network/NetworkManager.hpp"
#include "modules/FileReader/FileReaderManager.hpp"
#include "modules/ModError/ModErrorManager.hpp"
#include "modules/DirListing/DirListingManager.hpp"
#include "utils/macros.hpp"
#include "utils/Path.hpp"

#include "ModuleManagerFactory.hpp"

using namespace ZHTTPD;

ModuleManagerFactory::ModuleManagerFactory()
{
    this->_builders["mod_network"] = &ModuleManagerFactory::_createNetworkModuleManager;
    this->_available_modules[ZHTTPD::API::CATEGORY::INPUTOUTPUT].push_back("mod_network");
    this->_builders["mod_filereader"] = &ModuleManagerFactory::_createFileReaderModuleManager;
    this->_available_modules[ZHTTPD::API::CATEGORY::PROCESSING].push_back("mod_filereader");
    this->_builders["mod_error"] = &ModuleManagerFactory::_createModErrorModuleManager;
    this->_available_modules[ZHTTPD::API::CATEGORY::PROCESSING].push_back("mod_error");
    this->_builders["mod_dirlisting"] = &ModuleManagerFactory::_createDirListingModuleManager;
    this->_available_modules[ZHTTPD::API::CATEGORY::PROCESSING].push_back("mod_dirlisting");
}

ModuleManagerFactory::~ModuleManagerFactory()
{
    std::map<std::string, Library*>::iterator it = this->_libraries.begin(),
                                              end = this->_libraries.end();
    for (; it != end; ++it)
    {
        ZHTTPD_DELETE(it->second);
    }
}

void ModuleManagerFactory::findModules(std::string const& modules_directory)
{
    assert(modules_directory.size() > 0 && "modules_directory is not a valid path");
    Path dir(modules_directory);
    if (!dir.exists())
        throw std::runtime_error("The path '" + modules_directory +"' does not exist");
    if (!dir.isDirectory())
        throw std::runtime_error("The path '"+ modules_directory +"'is not a valid directory");
    std::list<std::string> files = dir.getDirectoryContent();
    for (std::list<std::string>::iterator it = files.begin(), end = files.end(); it != end; ++it)
    {
        if (this->_isDynamicLibrary(*it))
        {
            Library* lib = 0;
            try
            {
                lib = new Library(dir + *it);
                lib_handler_t getInstance = lib->resolve<lib_handler_t>("getInstance");
                ZHTTPD::API::IModuleManager* manager = getInstance();
                std::string const& name = manager->getName();
                // TODO rendre possible de remplacer les modules par défaut
                // comme mod_network, mod_error, etc...
                if (this->_builders.find(name) != this->_builders.end())
                {
                    throw std::runtime_error("Two modules with the name '" +
                                             name + "' have been found");
                }
                this->_libraries[name] = lib;
                this->_builders[name] = &ModuleManagerFactory::_createFromLibrary;
                this->_available_modules[manager->getCategory()].push_back(name);
                ZHTTPD_DELETE(manager);
            }
            catch (std::exception& err)
            {
                LOG_ERROR("Cannot load '" + std::string(dir + *it) + "': " + err.what());
                ZHTTPD_DELETE(lib);
            }
        }
    }
}

API::IModuleManager* ModuleManagerFactory::getModuleManager(std::string const& name) const
{
    std::map<std::string, creator_t>::const_iterator it = this->_builders.find(name);
    if (it != this->_builders.end())
    {
        return (this->*(it->second))(name);
    }
    LOG_WARN("Module '" + name + "' is not in the factory");
    return 0;
}

ModuleManagerFactory::available_modules_t const& ModuleManagerFactory::getAvailableModules() const
{
    return this->_available_modules;
}


API::IModuleManager* ModuleManagerFactory::_createNetworkModuleManager(std::string const&) const
{
    return new MOD::NetworkManager();
}

API::IModuleManager* ModuleManagerFactory::_createFileReaderModuleManager(std::string const&) const
{
    return new MOD::FileReaderManager();
}

API::IModuleManager* ModuleManagerFactory::_createModErrorModuleManager(std::string const&) const
{
    return new MOD::ModErrorManager();
}

API::IModuleManager* ModuleManagerFactory::_createDirListingModuleManager(std::string const&) const
{
    return new MOD::DirListingManager();
}

API::IModuleManager* ModuleManagerFactory::_createFromLibrary(std::string const& name) const
{
    std::map<std::string, Library*>::const_iterator it = this->_libraries.find(name);
    assert(it != this->_libraries.end());
    lib_handler_t getInstance = it->second->resolve<lib_handler_t>("getInstance");
    return getInstance();
}

#ifdef _WIN32
# define ZHTTPD_LIB_EXTENSION "dll"
#else
# define ZHTTPD_LIB_EXTENSION "so"
#endif

bool ModuleManagerFactory::_isDynamicLibrary(std::string const& filename) const
{
    std::size_t i_dot = filename.find_last_of(".");
    if (i_dot != std::string::npos && i_dot < filename.size() - 1)
        return (filename.substr(i_dot + 1) == ZHTTPD_LIB_EXTENSION);
    return false;
}

#undef ZHTTPD_LIB_EXTENSION


