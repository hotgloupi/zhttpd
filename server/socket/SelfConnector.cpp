
#ifdef _WIN32 // WINDOWS

# include <Winsock2.h>

#else // UNIX

# include <stdio.h>
# include <unistd.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <string.h>

#endif

#include <stdexcept>

#include "socket/SelfConnector.hpp"
#include "thread/ScopeLock.hpp"
#include "utils/macros.hpp"

using namespace ZHTTPD;

SelfConnector::SelfConnector(API::uint16_t port) : _port(port), _socket(0)
{
}

SelfConnector::~SelfConnector()
{
    ZHTTPD_DELETE(this->_socket);
}

#ifdef _WIN32 // WINDOWS

void SelfConnector::run()
{
    WSADATA wsaData;
    int err = ::WSAStartup(MAKEWORD(2,2), &wsaData);
    if (err)
        throw std::runtime_error("WSAStartup() failure: ");

    struct protoent *pe;
    struct sockaddr_in sin;
    struct hostent *h;
    API::socket_t fd;

    ScopeLock lock(this->_mutex);
    if ((h = gethostbyname("localhost")) == NULL)
        throw std::runtime_error("gethostbyname()");
    if ((pe = getprotobyname("tcp")) == NULL)
        throw std::runtime_error("getprotobyname()");
    if ((fd = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
        throw std::runtime_error("socket()");
    ::memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(this->_port);
    ::memcpy(&(sin.sin_addr), h->h_addr, sizeof(sin.sin_addr));
    if (connect(fd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1)
        throw std::runtime_error("connect");
    ZHTTPD_DELETE(this->_socket);
    this->_socket = new Socket(fd);
}

#else // UNIX

void SelfConnector::run()
{
    struct protoent *pe;
    struct sockaddr_in sin;
    struct hostent *h;
    API::socket_t fd;

    ScopeLock lock(this->_mutex);

    if ((h = gethostbyname("localhost")) == NULL)
        throw std::runtime_error("gethostbyname()");
    if ((pe = getprotobyname("tcp")) == NULL)
        throw std::runtime_error("getprotobyname()");
    if ((fd = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
        throw std::runtime_error("socket()");
    ::memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(this->_port);
    memcpy(&(sin.sin_addr), h->h_addr, sizeof(sin.sin_addr));
    if (connect(fd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) == -1)
        throw std::runtime_error("connect");
    ZHTTPD_DELETE(this->_socket);
    this->_socket = new Socket(fd);
}

#endif

Socket* SelfConnector::getSocket()
{
    ScopeLock lock(this->_mutex);
    Socket* socket = this->_socket;
    this->_socket = 0;
    return socket;
}
