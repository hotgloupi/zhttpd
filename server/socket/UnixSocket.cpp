
#ifndef WIN32
# include <stdexcept>
# include <iostream>
# include <errno.h>
# include <cstring>

# include <netinet/in.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>

# include "utils/Logger.hpp"
# include "UnixSocket.hpp"

using namespace zhttpd::implementation;

UnixSocket::UnixSocket(api::socket_t socket) : _socket(socket)
{
# ifdef SO_NOSIGPIPE12
    int set = 1;
    setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
# endif
}

UnixSocket::UnixSocket(uint32_t ip, uint16_t port)
{
    struct ::protoent *pe;
    struct sockaddr_in sin;

    if ((pe = ::getprotobyname("tcp")) == 0)
        throw std::runtime_error("getprotobyname() failure");
    if ((this->_socket = ::socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
        throw std::runtime_error("socket() failure");
    ::memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(ip);
    int yes = 1;
    if (::setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        throw std::runtime_error("setsockopt() failure");
    if (::bind(this->_socket, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) == -1)
        throw std::runtime_error("bind() failure " + std::string(strerror(errno)));
    if (::listen(this->_socket, 23) == -1)
        throw std::runtime_error("listen() failure");
}

UnixSocket::~UnixSocket()
{
    this->close();
}

void UnixSocket::close()
{
    ::close(this->_socket);
}

zhttpd::api::size_t UnixSocket::write(char const* data, size_t len)
{
    ssize_t res = ::send(this->_socket, data, len, MSG_NOSIGNAL);
    if (res <= 0)
    {
        throw std::runtime_error("write() failure: " + std::string(strerror(errno)));
    }
    return res;
}

zhttpd::api::size_t UnixSocket::read(char* buf, size_t len)
{
    ssize_t res = ::read(this->_socket, buf, len);
    if (res <= 0)
        throw std::runtime_error("read() failure: " + std::string(strerror(errno)));
    return res;
}

zhttpd::api::socket_t UnixSocket::accept()
{
    struct ::sockaddr_in csin;
    unsigned int len = sizeof(csin);
    api::socket_t socket = ::accept(this->_socket,
                               reinterpret_cast<struct ::sockaddr*>(&csin),
                               &len);
    if (socket < 0 || socket > 49999)
        throw std::runtime_error("accept() failure: " + std::string(strerror(errno)));

    return socket;
}

#endif // !WIN32
