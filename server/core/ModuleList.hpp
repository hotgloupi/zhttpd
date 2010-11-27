
#ifndef __MODULELIST_HPP__
# define __MODULELIST_HPP__

# include "api/IModuleManager.hpp"
# include "api/IModule.hpp"

# include "ModuleWrapper.hpp"

namespace zhttpd
{
    class ModuleList
    {
    private:
        ModuleWrapper* _head;
        ModuleWrapper* _tail;

    public:
        ModuleList();
        ~ModuleList();
        ModuleWrapper* append(api::IModuleManager& manager,
                              api::IModule& module);
        ModuleWrapper* setProcessingModule(api::IModuleManager& manager,
                                           api::IModule& module);
        ModuleWrapper* head();
        ModuleWrapper* tail();
        bool isInResponsePart(ModuleWrapper* node);
    };
}

#endif /* !__MODULELIST_HPP__ */

