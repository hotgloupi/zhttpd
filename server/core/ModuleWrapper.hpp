
#ifndef __MODULEWRAPPER_HPP__
# define __MODULEWRAPPER_HPP__

# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"

namespace ZHTTPD
{
    class ModuleWrapper
    {
    private:
        API::IModuleManager& _manager;
        API::IModule& _module;
        ModuleWrapper* _next;

    public:
        ModuleWrapper(API::IModuleManager& manager, API::IModule& module);
        virtual ~ModuleWrapper();
        ModuleWrapper* getNext();
        void setNext(ModuleWrapper* next);
        API::IModule& getModule();
        API::IModuleManager& getModuleManager();
    };
}

#endif /* !__MODULEWRAPPER_HPP__ */

