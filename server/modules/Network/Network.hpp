
#ifndef __NETWORK_HPP__
# define __NETWORK_HPP__

# include <map>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"
# include "socket/Socket.hpp"


namespace zhttpd
{
    namespace mod
    {
        class Network : public api::IModule
        {
        private:
            typedef bool (Network::*action_t)(api::IRequest* request, api::IBuffer* buffer);
            static action_t const _actions[api::event::ON_END + 1];

        private:
            bool        _processOnCanRead(api::IRequest* request, api::IBuffer* buffer);
            bool        _processOnCanWrite(api::IRequest* request, api::IBuffer* buffer);
            bool        _processOnResponseData(api::IRequest* request, api::IBuffer* buffer);

        public:
            static api::size_t const                BUFFER_SIZE;

        public:
            Network(api::IModuleManager* manager);
            ~Network();
            bool processRequest(api::event::Type event, api::IRequest* request, api::IBuffer* buffer);
        };
    }
}

#endif // __NETWORK_HPP__
