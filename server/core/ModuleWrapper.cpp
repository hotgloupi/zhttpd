
#include <cassert>

#include "utils/Logger.hpp"

#include "ModuleWrapper.hpp"

using namespace zhttpd;

ModuleWrapper::ModuleWrapper(api::IModuleManager& manager, api::IModule& module) :
    _manager(manager), _module(module), _next(0)
{
}

ModuleWrapper::~ModuleWrapper()
{
    this->_manager.releaseInstance(&this->_module);
}

ModuleWrapper* ModuleWrapper::getNext()
{
    return this->_next;
}

void ModuleWrapper::setNext(ModuleWrapper* next)
{
    assert(this->_next == 0 || next == 0);
    this->_next = next;
}

api::IModule& ModuleWrapper::getModule()
{
    return this->_module;
}

api::IModuleManager& ModuleWrapper::getModuleManager()
{
    return this->_manager;
}
