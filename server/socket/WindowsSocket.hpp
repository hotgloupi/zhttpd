
#ifndef __WINDOWSSOCKET_HPP__
# define __WINDOWSSOCKET_HPP__

# include <Windows.h>
# include "api/types.hpp"

namespace zhttpd
{
    namespace implementation
    {
        class WindowsSocket
        {
        protected:
            api::socket_t _socket;

        protected:
            api::socket_t accept();

        public:
            WindowsSocket(api::socket_t socket);
            WindowsSocket(api::uint32_t ip, api::uint16_t port);
            virtual ~WindowsSocket();
            void close();
            api::size_t write(char const* data, size_t len);
            api::size_t read(char* buf, size_t len);
        };

        typedef WindowsSocket       Socket;
    }
}

#endif /* !__WINDOWSSOCKET_HPP__ */

