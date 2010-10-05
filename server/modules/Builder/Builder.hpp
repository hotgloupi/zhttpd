
#ifndef __BUILDER_HPP__
# define __BUILDER_HPP__

# include "api/IModuleManager.hpp"
# include "api/IModule.hpp"
# include "configuration/Configuration.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class Builder : public ZHTTPD::API::IModule
        {
        private:
            bool _builded;

            void _findModules(ZHTTPD::API::IRequest* request, Configuration* config, VHost* vhost);
            void _addModule(ZHTTPD::API::CATEGORY::Type category, Configuration* config, VHost* vhost, ZHTTPD::API::IRequest* request);
            void _setPath(ZHTTPD::API::IRequest* request, Configuration* config, VHost* vhost);

        public:
            Builder(ZHTTPD::API::IModuleManager* manager);
            bool processRequest(ZHTTPD::API::EVENT::Type event,
                                ZHTTPD::API::IRequest* request,
                                ZHTTPD::API::IBuffer* buffer);
        };
    }
}

#endif /* !__BUILDER_HPP__ */

