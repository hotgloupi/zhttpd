
#include "api/IRequest.hpp"
#include "utils/Logger.hpp"
#include "core/ConfigurationManager.hpp"

#include "VHost.hpp"

using namespace zhttpd;

VHost::VHost(VHost* parent, Configuration& conf) :
    _configuration(conf),
    _childs(),
    _parent(parent),
    _name(),
    _document_root(),
    _modules(),
    _guide_lines()
{}

VHost::~VHost()
{
    for (std::list<VHost*>::iterator it = this->_childs.begin(),
         ite = this->_childs.end() ; it != ite ; ++it)
        delete *it;
    for (std::map<std::string, ModuleConfiguration*>::iterator it = this->_modules.begin(),
         ite = this->_modules.end() ; it != ite ; ++it)
        delete it->second;
    for (std::list<IVHostGuideline*>::iterator it = this->_guide_lines.begin(),
         ite = this->_guide_lines.end() ; it != ite ; ++it)
        delete *it;
}

VHost* VHost::match(api::IRequest& request)
{
    VHost* match = this;
    std::list<IVHostGuideline*>::iterator it_gl = this->_guide_lines.begin();
    std::list<IVHostGuideline*>::iterator ite_gl = this->_guide_lines.end();
    while (it_gl != ite_gl)
    {
        if ((*it_gl)->match(request) == false)
            return 0;
        ++it_gl;
    }
    std::list<VHost*>::iterator it_vh = this->_childs.begin();
    std::list<VHost*>::iterator ite_vh = this->_childs.end();
    VHost* child;
    while (it_vh != ite_vh)
    {
        if ((child = (*it_vh)->match(request)) != 0)
            return child;
        ++it_vh;
    }
    return match;
}

void VHost::addModule(ModuleConfiguration* module)
{
    this->_modules[module->getName()] = module;
}

zhttpd::api::IModuleManager* VHost::getModule(std::string const& name)
{
    if (this->_modules.find(name) != this->_modules.end())
    {
        if (this->_modules[name]->isEnabled())
        {
            if (this->_modules[name]->isMerged() == false)
            {
                this->_modules[name]->merge(this->_parent->getModuleConfiguration(name));
                this->_modules[name]->setMerged();
            }
            return this->_modules[name]->getModuleManager();
        }
    }
    else if (this->_parent != 0)
    {
        return this->_parent->getModule(name);
    }
    else
    {
        return this->_configuration.getModuleManager(name);
    }
    return (0);
}

bool VHost::moduleExists(std::string const& name)
{
    if (this->_modules.find(name) != this->_modules.end())
        return true;
    return false;
}

ModuleConfiguration& VHost::getModuleConfiguration(std::string const& modname)
{
    std::map<std::string, ModuleConfiguration*>::iterator it = this->_modules.find(modname);
    if (it == this->_modules.end())
    {
        ModuleConfiguration& base_conf = (this->_parent != 0 ?
                                          this->_parent->getModuleConfiguration(modname) :
                                          this->_configuration.getModuleConfiguration(modname));
        this->_modules[modname] = new ModuleConfiguration(modname, base_conf.isEnabled(), this->_configuration);
        this->_modules[modname]->merge(base_conf);
        return *(this->_modules[modname]);
    }
    return *(it->second);
}

void VHost::addGuideline(IVHostGuideline* guideLine)
{
    this->_guide_lines.push_back(guideLine);
}

void VHost::addChild(VHost* vhost)
{
    this->_childs.push_back(vhost);
}

void VHost::setDocumentRoot(std::string const& path)
{
    this->_document_root = path;
}

std::string const& VHost::getName() const
{
    return this->_name;
}

void VHost::setName(std::string const& name)
{
    this->_name = name;
}

std::string const& VHost::getDocumentRoot()
{
    if (this->_document_root == "" && this->_parent != 0)
        this->_document_root = this->_parent->getDocumentRoot();
    return this->_document_root;
}
