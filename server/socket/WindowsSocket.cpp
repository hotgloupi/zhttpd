
#ifdef _WIN32

# include <stdexcept>

# include "WindowsSocket.hpp"

using namespace zhttpd;
using namespace zhttpd::implementation;

WindowsSocket::WindowsSocket(api::socket_t socket) : _socket(socket)
{
}

WindowsSocket::WindowsSocket(zhttpd::api::uint32_t ip, zhttpd::api::uint16_t port)
{
    WSADATA wsaData;
    int err = ::WSAStartup(MAKEWORD(2,2), &wsaData);
    if (err)
        throw std::runtime_error("WSAStartup() failure: ");

    struct ::protoent *pe;
    struct ::sockaddr_in sin;

    if ((pe = ::getprotobyname("tcp")) == 0)
        throw std::runtime_error("tcp: getprotobyname() failure: " + ::WSAGetLastError());
    if ((this->_socket = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
        throw std::runtime_error("socket() failure");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(ip);
    int yes = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&yes), sizeof(int)) == -1)
        throw std::runtime_error("setsockopt() failure");
    if (bind(this->_socket, reinterpret_cast<struct sockaddr *>(&sin), sizeof(sin)) == -1)
        throw std::runtime_error("bind() failure");
    if (listen(this->_socket, 23) == -1)
        throw std::runtime_error("listen() failure");
}

WindowsSocket::~WindowsSocket()
{
    this->close();
}

void WindowsSocket::close()
{
    ::closesocket(this->_socket);
}

zhttpd::api::size_t WindowsSocket::write(char const* data, size_t len)
{
    zhttpd::api::ssize_t res = ::send(this->_socket, data, len, 0);
    if (res < 0)
        throw std::runtime_error("write() failure");
    return res;
}

zhttpd::api::size_t WindowsSocket::read(char* data, size_t len)
{
    zhttpd::api::ssize_t res = ::recv(this->_socket, data, len, 0);
    // TODO
    // Je m'appelle Martin et je viens de changer ca
    // ca throw quand ca deco (c'est mieux ? ou pas)
    if (res <= 0)
    // if (res < 0)
        throw std::runtime_error("read() failure");
    return res;
}

zhttpd::api::socket_t WindowsSocket::accept()
{
    struct ::sockaddr_in csin;
    unsigned int len = sizeof(csin);
    zhttpd::api::socket_t socket = ::accept(this->_socket,
                                         reinterpret_cast<struct ::sockaddr*>(&csin),
                                         reinterpret_cast<int *>(&len));
    if (socket < 0)
        throw std::runtime_error("accept() failure");
    return socket;
}

#endif // _WIN32
