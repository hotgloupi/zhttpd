
#ifndef __WINDOWSSOCKET_HPP__
# define __WINDOWSSOCKET_HPP__

# include <Windows.h>
# include "api/types.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class WindowsSocket
        {
        protected:
            API::socket_t _socket;

        protected:
            API::socket_t accept();

        public:
            WindowsSocket(API::socket_t socket);
            WindowsSocket(API::uint32_t ip, API::uint16_t port);
            virtual ~WindowsSocket();
            void close();
            API::size_t write(char const* data, size_t len);
            API::size_t read(char* buf, size_t len);
        };

        typedef WindowsSocket       Socket;
    }
}

#endif /* !__WINDOWSSOCKET_HPP__ */

