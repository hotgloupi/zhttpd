
#ifndef __BUILDER_HPP__
# define __BUILDER_HPP__

# include "api/IModuleManager.hpp"
# include "api/IModule.hpp"
# include "configuration/Configuration.hpp"

namespace zhttpd
{
    namespace mod
    {
        class Builder : public zhttpd::api::IModule
        {
        private:
            bool _builded;

            void _findModules(zhttpd::api::IRequest* request, Configuration* config, VHost* vhost);
            void _addModule(zhttpd::api::category::Type category, Configuration* config, VHost* vhost, zhttpd::api::IRequest* request);
            void _setPath(zhttpd::api::IRequest* request, Configuration* config, VHost* vhost);

        public:
            Builder(zhttpd::api::IModuleManager* manager);
            bool processRequest(zhttpd::api::event::Type event,
                                zhttpd::api::IRequest* request,
                                zhttpd::api::IBuffer* buffer);
        };
    }
}

#endif /* !__BUILDER_HPP__ */

