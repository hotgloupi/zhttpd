
#ifndef __UNIXSOCKET_HPP__
# define __UNIXSOCKET_HPP__

# include "api/types.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class UnixSocket
        {
        protected:
            API::socket_t _socket;

        protected:
            API::socket_t accept();

        public:
            UnixSocket(API::socket_t socket);
            UnixSocket(uint32_t ip, uint16_t port);
            virtual ~UnixSocket();
            void close();
            API::size_t write(char const* data, size_t len);
            API::size_t read(char* buf, size_t len);
        };

        typedef UnixSocket          Socket;
    }
}

#endif /* !__UNIXSOCKET_HPP__ */

