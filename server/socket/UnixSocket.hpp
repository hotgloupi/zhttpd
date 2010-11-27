
#ifndef __UNIXSOCKET_HPP__
# define __UNIXSOCKET_HPP__

# include "api/types.hpp"

namespace zhttpd
{
    namespace implementation
    {
        class UnixSocket
        {
        protected:
            api::socket_t _socket;

        protected:
            api::socket_t accept();

        public:
            UnixSocket(api::socket_t socket);
            UnixSocket(uint32_t ip, uint16_t port);
            virtual ~UnixSocket();
            void close();
            api::size_t write(char const* data, size_t len);
            api::size_t read(char* buf, size_t len);
        };

        typedef UnixSocket          Socket;
    }
}

#endif /* !__UNIXSOCKET_HPP__ */

