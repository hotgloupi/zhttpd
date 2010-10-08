
#ifdef _WIN32
# define LIB_EXTENSION "dll"
#else
# define LIB_EXTENSION "so"
#endif

#include <stdexcept>

#include "utils/macros.hpp"
#include "modules/Network/NetworkManager.hpp"

#include "Configuration.hpp"

using namespace ZHTTPD;

Configuration::Configuration() :
    _modules_configuration(),
    _factory(),
    _index_files(),
    _vhost(0),
    _ports()
{
}

Configuration::~Configuration()
{
    LOG_INFO("Unloading configuration");
    delete this->_vhost;
    modules_configuration_t::iterator it = this->_modules_configuration.begin();
    modules_configuration_t::iterator end = this->_modules_configuration.end();
    for (; it != end ; ++it)
    {
        delete it->second;
    }
}

VHost& Configuration::getVHost() const
{
    if (this->_vhost == 0)
        throw std::runtime_error("The configuration has no root vhost !");
    return *this->_vhost;
}

void Configuration::addModuleConfiguration(ModuleConfiguration& module_configuration)
{
    std::string const& name = module_configuration.getName();

    if (this->_modules_configuration.find(name) != this->_modules_configuration.end())
    {
        LOG_WARN("Module '" + name + "' added twice !");
        ZHTTPD_DELETE(this->_modules_configuration[name]);
    }
    this->_modules_configuration[name] = &module_configuration;
}

Configuration::modules_configuration_t const& Configuration::getModulesConfiguration() const
{
    return this->_modules_configuration;
}

ModuleConfiguration& Configuration::getModuleConfiguration(std::string const& name)
{
    modules_configuration_t::const_iterator it = this->_modules_configuration.find(name);
    if (it == this->_modules_configuration.end())
    {
        bool enabled = (name == "mod_error"); //XXX this is ugly, only mod_error is enabled by default
        this->_modules_configuration[name] = new ModuleConfiguration(name, enabled, *this);
        return *this->_modules_configuration[name];
    }
    return *it->second;
}

API::IModuleManager* Configuration::getModuleManager(std::string const& name)
{
    return this->_factory.getModuleManager(name);
}

Configuration::available_modules_t const& Configuration::getAvailableModules() const
{
    return this->_factory.getAvailableModules();
}

Configuration::io_modules_t const& Configuration::getListenPorts() const
{
    return this->_ports;
}

void Configuration::addListenPort(API::uint16_t port, std::string const& io_module)
{
    if (this->_ports.find(port) != this->_ports.end())
        throw std::runtime_error("Cannot bind listen port twice");
    if (io_module.size() == 0)
    {
        this->_ports[port] = "mod_network";
        if (this->_modules_configuration.find("mod_network") == this->_modules_configuration.end())
        {
            ModuleConfiguration* conf = new ModuleConfiguration("mod_network", true, *this);
            this->addModuleConfiguration(*conf);
        }
    }
    else
        this->_ports[port] = io_module;
}

void Configuration::setVHost(VHost& vhost)
{
    // XXX check already exist
    this->_vhost = &vhost;
}

API::IModuleManager& Configuration::getInputOutputModuleManager(API::uint16_t port)
{
    io_modules_t::iterator it = this->_ports.find(port);
    if (it == this->_ports.end())
        throw std::runtime_error("port '" + Logger::toString(port) + "' is not described in the configuration!");
    modules_configuration_t::iterator conf = this->_modules_configuration.find(it->second);
    if (conf == this->_modules_configuration.end())
        throw std::runtime_error("port '" + Logger::toString(port) + "' is binded with '" +
                                 it->second + "', which has no configuration");
    API::IModuleManager* manager = conf->second->getModuleManager();
    if (manager == 0)
        throw std::runtime_error("The module '" + it->second + "' has no manager");
    return *manager;
}

void Configuration::addModuleDirectory(std::string const& path)
{
    this->_factory.findModules(path);
}

void Configuration::addIndexFile(std::string const& filename)
{
    this->_index_files.push_back(filename);
}

std::list<std::string> const& Configuration::getIndexFiles() const
{
    return this->_index_files;
}

//
//
// bool Configuration::loadModules()
// {
//     LOG_INFO("Loading modules");
//     if (this->_modules_directory != 0)
//     {
//         if (this->_modules_directory->exists())
//         {
//             std::list<std::string> modulesFiles = this->_modules_directory->getDirectoryFiles();
//             std::list<std::string>::iterator it = modulesFiles.begin();
//             std::list<std::string>::iterator ite = modulesFiles.end();
//             while (it != ite)
//             {
//                 if (this->_isDynamicLibrary(*it))
//                     this->_loadModule(*it);
//                 ++it;
//             }
//             LOG_DEBUG("Modules loaded from " + ZHTTPD::IMPLEMENTATION::FileSystem::cwd() + '/' + this->_modules_directory->getPath());
//         }
//         else
//             LOG_WARN("Directory " + this->_modules_directory->getPath() + " does not exists. Cannot load modules");
//     }
//     else
//         LOG_WARN("No directory has been specified for modules");
//     return (false);
// }
//
// // TODO a deplacer
// bool Configuration::_isDynamicLibrary(std::string const& filename)
// {
//     std::size_t i_dot = filename.find_last_of(".");
//     if (i_dot != std::string::npos && i_dot < filename.size() - 1)
//         return (filename.substr(i_dot + 1) == LIB_EXTENSION);
//     return false;
// }
//
// bool Configuration::_loadModule(std::string const& filename)
// {
//     LOG_INFO("Loading " + this->_modules_directory->getPath() + filename);
//
//     try
//     {
//         std::string module_name = filename.substr(0, filename.find_last_of("."));
//         ZHTTPD::Library* lib = new Library(this->_modules_directory->getPath() + module_name);
//         lib_handler_t getInstance = lib->resolve<lib_handler_t>("getInstance");
//         ZHTTPD::API::IModuleManager* manager = getInstance();
//         this->_available_modules[manager->getCategory()].push_back(manager->getName());
//         this->_modules_handlers[manager->getName()] = lib;
//         ZHTTPD_DELETE(manager);
//         return true;
//     }
//     catch (std::exception& e)
//     {
//         LOG_WARN(e.what());
//     }
//     return false;
// }
//
// API::IModuleManager* Configuration::getModuleManager(std::string const& name)
// {
//     if (this->_modules.find(name) != this->_modules.end() && this->_modules[name]->isEnabled())
//         return this->_modules[name]->getModuleManager();
//     return 0;
// }
//
// VHost* Configuration::getVHost() const
// {
//     return (this->_vhost);
// }
//
// void Configuration::addModule(ModuleConfiguration* module)
// {
//     assert(module != 0);
//     if (this->_modules.find(module->getName()) != this->_modules.end())
//         delete module;
//     else
//         this->_modules[module->getName()] = module;
// }
//
// std::map<std::string, ModuleConfiguration*> const& Configuration::getModules() const
// {
//     return this->_modules;
// }
//
// ModuleConfiguration* Configuration::getModuleConfiguration(std::string const& name)
// {
//     if (this->_modules.find(name) != this->_modules.end() && this->_modules[name]->isEnabled())
//         return this->_modules[name];
//     return 0;
// }
//
// std::map<API::uint16_t, std::string> const& Configuration::getListenPorts() const
// {
//     return this->_ports;
// }
//
// void Configuration::addListenPort(API::uint16_t port, std::string const& io_module)
// {
//     this->_ports[port] = io_module;
// }
//
// void Configuration::setVHost(VHost* vhost)
// {
//     this->_vhost = vhost;
// }
//
// void Configuration::setModulesDirectory(std::string const& path)
// {
//     ZHTTPD_DELETE(this->_modules_directory);
//     this->_modules_directory = new Path(path);
// }
//
// void Configuration::setModulesDirectory(Path* path)
// {
//     ZHTTPD_DELETE(this->_modules_directory);
//     this->_modules_directory = path;
// }
//
// Configuration::available_modules_map_t& Configuration::getAvailableModules()
// {
//     return this->_available_modules;
// }
//
// Library* Configuration::getLibHandler(std::string const& name)
// {
//     if (this->_modules_handlers.find(name) != this->_modules_handlers.end())
//         return this->_modules_handlers[name];
//     return 0;
// }
//
// API::IModuleManager* Configuration::getInputOutputModuleManager(API::uint16_t port)
// {
//     if (this->_io_module_managers.find(port) != this->_io_module_managers.end())
//         return this->_io_module_managers[port];
//     else
//     {
//         if (this->_ports.find(port) != this->_ports.end())
//             return this->loadIOModuleManager(port, this->_ports[port]);
//         else
//             return this->loadIOModuleManager(port, "");
//     }
// }
//
// void Configuration::addIndexFile(std::string const& filename)
// {
//     this->_index_files.push_back(filename);
// }
//
// std::list<std::string> const& Configuration::getIndexFiles()
// {
//     return this->_index_files;
// }
//
// API::IModuleManager* Configuration::loadIOModuleManager(API::uint16_t port, std::string const& name)
// {
//     if (name != "")
//     {
//         LOG_DEBUG("loading " + name + " as io_module");
//         API::IModuleManager* manager = this->getModuleManager(name);
//         if (manager != 0 && manager->getCategory() == API::CATEGORY::INPUTOUTPUT)
//         {
//             this->_io_module_managers[port] = manager;
//             return this->_io_module_managers[port];
//         }
//     }
//
//     LOG_DEBUG("loading default io_module");
//     this->_io_module_managers[port] = new MOD::NetworkManager();
//     return this->_io_module_managers[port];
// }
