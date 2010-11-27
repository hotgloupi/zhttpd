
#ifndef __HOSTADDRESS_HPP__
# define __HOSTADDRESS_HPP__

# include <string>

# include "api/types.hpp"

namespace zhttpd
{
    class HostAddress
    {
    private:
        api::uint32_t _ip;
        api::uint16_t _port;
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
        HostAddress(api::uint32_t ip, api::uint16_t port);
        HostAddress(std::string const& host);
        HostAddress(api::socket_t socket);

        api::uint32_t getIp();
        api::uint16_t getPort();

        std::string getHostname();
    };
}

#endif /* !__HOSTADDRESS_HPP__ */

