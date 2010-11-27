
#include <cassert>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "utils/Logger.hpp"
#include "core/Request.hpp"
#include "utils/StatsManager.hpp"

#include "Network.hpp"

using namespace ZHTTPD::MOD;

ZHTTPD::API::size_t const Network::BUFFER_SIZE = 2048;

Network::action_t const Network::_actions[ZHTTPD::API::EVENT::ON_ERROR] = {
    &Network::_processOnCanRead,            // ON_CAN_READ         = 1,
    &Network::_processOnCanWrite,           // ON_CAN_WRITE        = 2,
    0,
    &Network::_processOnResponseData,       // ON_RESPONSE_DATA    = 4,
    0,
    0,
    0,
};

Network::Network(ZHTTPD::API::IModuleManager*)
{
}

Network::~Network()
{
}

bool Network::processRequest(ZHTTPD::API::EVENT::Type event, ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buffer)
{
    if (Network::_actions[event - 1] != 0)
    {
        action_t action = Network::_actions[event - 1];
        return (this->*action)(request, buffer);
    }
    return (false);
}

bool Network::_processOnCanRead(ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buf)
{
    ZHTTPD::Socket& socket = reinterpret_cast<Request*>(request)->getServerSession().getServerSocket();
    assert(buf == 0);
    ZHTTPD::API::IBuffer* buffer = request->getBufferManager().allocate(Network::BUFFER_SIZE);
    ZHTTPD::API::size_t bytes_read = socket.read(buffer->getRawData(), Network::BUFFER_SIZE);
    if (bytes_read < Network::BUFFER_SIZE)
    {
        buffer->setSize(bytes_read);
    }
    request->giveData(buffer);
    StatsManager::getInstance()->addReceivedBytes(buffer->getSize());

    return (true);
}

bool Network::_processOnCanWrite(ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buffer)
{
    assert(buffer != 0);
    assert(buffer->getSize() > 0);
    ZHTTPD::Socket& socket = reinterpret_cast<Request*>(request)->getServerSession().getServerSocket();
    LOG_FATAL("Write '" + std::string(buffer->getRawData(), buffer->getSize()) + "'");
    ZHTTPD::API::size_t bytes_sent = socket.write(buffer->getRawData(), buffer->getSize());
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

bool Network::_processOnResponseData(ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buffer)
{
    assert(buffer != 0);
    assert(buffer->getSize() > 0);
    request->needWrite(buffer);
    return (true);
}

