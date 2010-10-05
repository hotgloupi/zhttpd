
#include <cassert>
#include <stdexcept>

#include <openssl/err.h>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"

#include "SSLNetwork.hpp"
#include "SSLNetworkManager.hpp"
#include "utils/Logger.hpp"

using namespace ZHTTPD;
using namespace ZHTTPD::MOD;

API::size_t const SSLNetwork::BUFFER_SIZE = 2048;

SSLNetwork::action_t const SSLNetwork::_actions[API::EVENT::ON_ERROR] = {
    &SSLNetwork::_processOnCanRead,             // ON_CAN_READ          = 1,
    &SSLNetwork::_processOnCanWrite,            // ON_CAN_WRITE         = 2,
    0,
    &SSLNetwork::_processOnResponseData,        // ON_RESPONSE_DATA     = 4,
    0,
    &SSLNetwork::_processOnEnd,                 // ON_END               = 6
    0,
};

bool    SSLNetwork::processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer)
{
    if (SSLNetwork::_actions[event - 1] == 0)
        return false;

    if (this->_ssl == 0)
    {
        if (this->_resp == false)
            this->_initSsl(request->getSession().getSocket());
        else
        {
            if ((this->_ssl = this->_manager.getSsl(request->getSession().getSocket())) == 0)
                throw std::runtime_error("No SSL in output SSLNetwork");
        }
    }

    return (this->*SSLNetwork::_actions[event - 1])(request, buffer);
}

bool    SSLNetwork::_processOnCanRead(API::IRequest* request, API::IBuffer* buf)
{
    assert(buf == 0);

    ZHTTPD::API::IBuffer* buffer = request->getBufferManager().allocate(SSLNetwork::BUFFER_SIZE);

    ZHTTPD::API::ssize_t r = SSL_read(this->_ssl, buffer->getRawData(), SSLNetwork::BUFFER_SIZE);
    if (r <= 0)
    {
        request->getBufferManager().release(buffer);
        throw std::runtime_error(Logger::toString("SSL_read error") + ERR_error_string(ERR_get_error(), 0));
    }

    ZHTTPD::API::size_t bytes_read = r;
    if (bytes_read < SSLNetwork::BUFFER_SIZE)
        buffer->setSize(bytes_read);
    request->giveData(buffer);

    return (true);
}

bool    SSLNetwork::_processOnCanWrite(API::IRequest* request, API::IBuffer* buffer)
{
    assert(buffer != 0);
    assert(buffer->getSize() > 0);

    ZHTTPD::API::ssize_t r = SSL_write(this->_ssl, buffer->getRawData(), buffer->getSize());
    if (r <= 0)
        throw std::runtime_error(Logger::toString("SSL_write error") + ERR_error_string(ERR_get_error(), 0));

    ZHTTPD::API::size_t bytes_sent = r;
    if (bytes_sent < buffer->getSize())
    {
        buffer->setSize(bytes_sent, buffer->getSize() - bytes_sent);
        request->needWrite(buffer);
    }
    else
        request->getBufferManager().release(buffer);

    return (true);
}

bool    SSLNetwork::_processOnResponseData(API::IRequest* request, API::IBuffer* buffer)
{
    assert(buffer != 0);
    assert(buffer->getSize() > 0);
    request->needWrite(buffer);
    return (true);
}

bool    SSLNetwork::_processOnEnd(API::IRequest* request, API::IBuffer*)
{
    if (this->_resp == true && this->_ssl != 0)
    {
//        SSL_shutdown(this->_ssl); TODO ON_END apres ON_WRITE svp
        this->_manager.delSsl(this->_ssl);
        request->raiseEnd();
        return true;
    }
    return false;
}

bool    SSLNetwork::_initSsl(API::socket_t fd)
{
    this->_ssl = SSL_new(this->_manager.getCtx());
    if (this->_ssl == 0)
        throw std::runtime_error(Logger::toString("SSL_new failure: ") + ERR_error_string(ERR_get_error(), 0));
    if (SSL_set_fd(this->_ssl, fd) == 0)
        throw std::runtime_error(Logger::toString("SSL_set_fd failure: ") + ERR_error_string(ERR_get_error(), 0));
    if (SSL_accept(this->_ssl) <= 0)
        throw std::runtime_error(Logger::toString("SSL_accept failure: ") + ERR_error_string(ERR_get_error(), 0));
    this->_manager.addSsl(fd, this->_ssl);
    return true;
}

SSLNetwork::SSLNetwork(API::IModuleManager* manager, bool resp) :
    _manager(*reinterpret_cast<SSLNetworkManager*>(manager)),
    _ssl(0),
    _resp(resp)
{
}

SSLNetwork::~SSLNetwork()
{
    if (this->_resp == true && this->_ssl != 0)
    {
        SSL_free(this->_ssl);
    }
}
