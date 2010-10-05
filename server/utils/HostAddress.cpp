
#include <sstream>
#include <stdexcept>

#ifndef _WIN32
# include <sys/socket.h>
# include <netinet/in.h>
#else
typedef int socklen_t;
#endif // !_WIN32

#include "HostAddress.hpp"

using namespace ZHTTPD;


HostAddress::HostAddress(API::uint32_t ip, API::uint16_t port) :
    _ip(ip), _port(port)
{
}

HostAddress::HostAddress(std::string const& host) :
    _ip(0), _port(0), _host(host)
{
    throw std::runtime_error("HostAddress::HostAddress(std::string const& host): Not implemented");
}

HostAddress::HostAddress(API::socket_t socket) :
    _ip(0), _port(0)
{
    struct ::sockaddr_in sa;
    API::size_t size = sizeof(sa);
    int res = ::getpeername(
        socket,
        reinterpret_cast<struct ::sockaddr*>(&sa),
        reinterpret_cast<socklen_t*>(&size)
    );
    if (res == HostAddress::GETPEERNAME_ERROR || size != sizeof(sa))
        throw std::runtime_error("getpeername() failure");
    this->_ip = ntohl(sa.sin_addr.s_addr);
    this->_port = ntohs(sa.sin_port);
}

API::uint32_t HostAddress::getIp()
{
    return this->_ip;
}

API::uint16_t HostAddress::getPort()
{
    return this->_port;
}

std::string HostAddress::getHostname()
{
    if (this->_host.size() == 0)
    {
        std::stringstream ss;
        ss << ((this->_ip & 0xFF000000) >> 24) << "."
           << ((this->_ip & 0x00FF0000) >> 16) << "."
           << ((this->_ip & 0x0000FF00) >> 8) << "."
           << ((this->_ip & 0x000000FF))
           << ":" << this->_port;
        this->_host = ss.str();
    }
    return this->_host;
}

