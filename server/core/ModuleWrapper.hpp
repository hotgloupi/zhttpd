
#ifndef __MODULEWRAPPER_HPP__
# define __MODULEWRAPPER_HPP__

# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"

namespace zhttpd
{
    class ModuleWrapper
    {
    private:
        api::IModuleManager& _manager;
        api::IModule& _module;
        ModuleWrapper* _next;

    public:
        ModuleWrapper(api::IModuleManager& manager, api::IModule& module);
        virtual ~ModuleWrapper();
        ModuleWrapper* getNext();
        void setNext(ModuleWrapper* next);
        api::IModule& getModule();
        api::IModuleManager& getModuleManager();
    };
}

#endif /* !__MODULEWRAPPER_HPP__ */

