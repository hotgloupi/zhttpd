
#ifndef __HOSTADDRESS_HPP__
# define __HOSTADDRESS_HPP__

# include <string>

# include "api/types.hpp"

namespace ZHTTPD
{
    class HostAddress
    {
    private:
        API::uint32_t _ip;
        API::uint16_t _port;
        std::string _host;
        enum
        {
# ifdef _WIN32
            GETPEERNAME_ERROR = SOCKET_ERROR,
# else
            GETPEERNAME_ERROR = -1,
# endif
        };
    public:
        HostAddress(API::uint32_t ip, API::uint16_t port);
        HostAddress(std::string const& host);
        HostAddress(API::socket_t socket);

        API::uint32_t getIp();
        API::uint16_t getPort();

        std::string getHostname();
    };
}

#endif /* !__HOSTADDRESS_HPP__ */

