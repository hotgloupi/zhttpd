
#include <cassert>

#include "utils/HostAddress.hpp"
#include "utils/macros.hpp"
#include "utils/Logger.hpp"

#include "Session.hpp"

using namespace ZHTTPD;

Session::Session(Socket* socket, API::uint16_t port) :
    _socket(socket),
    _ip(0),
    _port(port),
    _processed_request_count(0),
    _fd(socket->getSocket())
{
    assert(this->_socket != 0);
    assert(this->_port != 0);
}

Session::~Session()
{
    LOG_DEBUG("DELETE SESSION");
    ZHTTPD_DELETE(this->_socket);
}

API::socket_t Session::getSocket() const
{
    return this->_fd;
}

API::uint32_t Session::getIp() const
{
    if (this->_ip == 0)
    {
        this->_ip = HostAddress(this->_fd).getIp();
        assert(this->_ip != 0);
    }
    return this->_ip;
}

API::uint16_t Session::getPort() const
{
    assert(this->_port != 0);
    return this->_port;
}

void Session::incrProcessedRequestCount()
{
    ++this->_processed_request_count;
}

API::size_t Session::getProcessedRequestCount() const
{
    return this->_processed_request_count;
}

Socket& Session::getServerSocket()
{
    return *this->_socket;
}

Socket* Session::getServerSocketPtr()
{
    return this->_socket;
}

Socket& Session::popServerSocket()
{
    Socket& s = *this->_socket;
    this->_socket = 0;
    return s;
}
