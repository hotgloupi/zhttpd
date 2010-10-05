#ifndef __SSLNETWORK_HPP__
#define __SSLNETWORK_HPP__

# include <openssl/ssl.h>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class SSLNetworkManager;

        class SSLNetwork : public API::IModule
        {
        private:
            typedef bool (SSLNetwork::*action_t)(API::IRequest* request, API::IBuffer* buffer);
            static action_t const _actions[API::EVENT::ON_END + 1];

        private:
            bool _processOnCanRead(API::IRequest* request, API::IBuffer* buffer);
            bool _processOnCanWrite(API::IRequest* request, API::IBuffer* buffer);
            bool _processOnResponseData(API::IRequest* request, API::IBuffer* buffer);
            bool _processOnEnd(API::IRequest* request, API::IBuffer* buffer);

            bool _initSsl(API::socket_t fd);

        public:
            static API::size_t const BUFFER_SIZE;

        public:
            bool processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer);

            SSLNetwork(API::IModuleManager* manager, bool resp = false);
            ~SSLNetwork();

        private:
            SSLNetworkManager& _manager;
            SSL* _ssl;
            bool _resp;
        };

    }
}

#endif // __SSLNETWORK_HPP__
