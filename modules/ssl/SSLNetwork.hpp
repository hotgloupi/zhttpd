#ifndef __SSLNETWORK_HPP__
#define __SSLNETWORK_HPP__

# include <openssl/ssl.h>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"

namespace zhttpd
{
    namespace mod
    {
        class SSLNetworkManager;

        class SSLNetwork : public api::IModule
        {
        private:
            typedef bool (SSLNetwork::*action_t)(api::IRequest* request, api::IBuffer* buffer);
            static action_t const _actions[api::event::ON_END + 1];

        private:
            bool _processOnCanRead(api::IRequest* request, api::IBuffer* buffer);
            bool _processOnCanWrite(api::IRequest* request, api::IBuffer* buffer);
            bool _processOnResponseData(api::IRequest* request, api::IBuffer* buffer);
            bool _processOnEnd(api::IRequest* request, api::IBuffer* buffer);

            bool _initSsl(api::socket_t fd);

        public:
            static api::size_t const BUFFER_SIZE;

        public:
            bool processRequest(api::event::Type event, api::IRequest* request, api::IBuffer* buffer);

            SSLNetwork(api::IModuleManager* manager, bool resp = false);
            ~SSLNetwork();

        private:
            SSLNetworkManager& _manager;
            SSL* _ssl;
            bool _resp;
        };

    }
}

#endif // __SSLNETWORK_HPP__
