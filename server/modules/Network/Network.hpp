
#ifndef __NETWORK_HPP__
# define __NETWORK_HPP__

# include <map>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"
# include "socket/Socket.hpp"


namespace ZHTTPD
{
    namespace MOD
    {
        class Network : public API::IModule
        {
        private:
            typedef bool (Network::*action_t)(API::IRequest* request, API::IBuffer* buffer);
            static action_t const _actions[API::EVENT::ON_END + 1];

        private:
            bool        _processOnCanRead(API::IRequest* request, API::IBuffer* buffer);
            bool        _processOnCanWrite(API::IRequest* request, API::IBuffer* buffer);
            bool        _processOnResponseData(API::IRequest* request, API::IBuffer* buffer);

        public:
            static API::size_t const                BUFFER_SIZE;

        public:
            Network(API::IModuleManager* manager);
            ~Network();
            bool processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer);
        };
    }
}

#endif // __NETWORK_HPP__
