
#ifndef __SOCKET_HPP__
# define __SOCKET_HPP__

# ifdef WIN32
#  include "WindowsSocket.hpp"
# else
#  include "UnixSocket.hpp"
# endif

# include "utils/Logger.hpp"
# include "api/types.hpp"

namespace zhttpd
{
    class Socket : public implementation::Socket
    {
    private:
        Socket();
        Socket(Socket const&);

    public:
        Socket(api::socket_t socket) : implementation::Socket(socket)
        {
        }

        Socket(api::uint32_t ip, api::uint16_t port) : implementation::Socket(ip, port)
        {
        }

        api::socket_t getSocket() const
        {
            return this->_socket;
        }

        Socket* accept()
        {
            return new Socket(implementation::Socket::accept());
        }

        virtual ~Socket()
        {
            LOG_DEBUG("DELETE SOCKET");
        }
    };
}

#endif /* !__SOCKET_HPP__ */

