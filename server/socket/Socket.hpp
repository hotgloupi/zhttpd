
#ifndef __SOCKET_HPP__
# define __SOCKET_HPP__

# ifdef WIN32
#  include "WindowsSocket.hpp"
# else
#  include "UnixSocket.hpp"
# endif

# include "utils/Logger.hpp"
# include "api/types.hpp"

namespace ZHTTPD
{
    class Socket : public IMPLEMENTATION::Socket
    {
    private:
        Socket();
        Socket(Socket const&);

    public:
        Socket(API::socket_t socket) : IMPLEMENTATION::Socket(socket)
        {
        }

        Socket(API::uint32_t ip, API::uint16_t port) : IMPLEMENTATION::Socket(ip, port)
        {
        }

        API::socket_t getSocket() const
        {
            return this->_socket;
        }

        Socket* accept()
        {
            return new Socket(IMPLEMENTATION::Socket::accept());
        }

        virtual ~Socket()
        {
            LOG_DEBUG("DELETE SOCKET");
        }
    };
}

#endif /* !__SOCKET_HPP__ */

