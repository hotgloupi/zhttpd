
#include <cassert>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "utils/Logger.hpp"
#include "core/Request.hpp"
#include "utils/StatsManager.hpp"

#include "Network.hpp"

using namespace zhttpd::mod;

zhttpd::api::size_t const Network::BUFFER_SIZE = 2048;

Network::action_t const Network::_actions[zhttpd::api::event::ON_ERROR] = {
    &Network::_processOnCanRead,            // ON_CAN_READ         = 1,
    &Network::_processOnCanWrite,           // ON_CAN_WRITE        = 2,
    0,
    &Network::_processOnResponseData,       // ON_RESPONSE_DATA    = 4,
    0,
    0,
    0,
};

Network::Network(zhttpd::api::IModuleManager*)
{
}

Network::~Network()
{
}

bool Network::processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer)
{
    if (Network::_actions[event - 1] != 0)
    {
        action_t action = Network::_actions[event - 1];
        return (this->*action)(request, buffer);
    }
    return (false);
}

bool Network::_processOnCanRead(zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buf)
{
    zhttpd::Socket& socket = reinterpret_cast<Request*>(request)->getServerSession().getServerSocket();
    assert(buf == 0);
    zhttpd::api::IBuffer* buffer = request->getBufferManager().allocate(Network::BUFFER_SIZE);
    zhttpd::api::size_t bytes_read = socket.read(buffer->getRawData(), Network::BUFFER_SIZE);
    if (bytes_read < Network::BUFFER_SIZE)
    {
        buffer->setSize(bytes_read);
    }
    request->giveData(buffer);
    StatsManager::getInstance()->addReceivedBytes(buffer->getSize());

    return (true);
}

bool Network::_processOnCanWrite(zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer)
{
    assert(buffer != 0);
    assert(buffer->getSize() > 0);
    zhttpd::Socket& socket = reinterpret_cast<Request*>(request)->getServerSession().getServerSocket();
    zhttpd::api::size_t bytes_sent = socket.write(buffer->getRawData(), buffer->getSize());
    StatsManager::getInstance()->addSentBytes(buffer->getSize());
    if (bytes_sent < buffer->getSize())
    {
        buffer->setSize(bytes_sent, buffer->getSize() - bytes_sent);
        request->needWrite(buffer);
    }
    else
    {
        request->getBufferManager().release(buffer);
    }
    return (true);
}

bool Network::_processOnResponseData(zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer)
{
    assert(buffer != 0);
    assert(buffer->getSize() > 0);
    request->needWrite(buffer);
    return (true);
}

