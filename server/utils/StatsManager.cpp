
#include "utils/Atomic.hpp"
#include "utils/StatsManager.hpp"

namespace zhttpd
{
    StatsManager::StatsManager() :
        _sentBytes(0), _receivedBytes(0), _requests(0), _buffers(0), _memory(0)
    {
    }

    StatsManager::~StatsManager()
    {
    }

    void StatsManager::addMemory(int bytes)
    {
        zhttpd::api::size_t res = Atomic::add(&this->_memory, static_cast<unsigned int>(bytes));
        LOG_DEBUG(Logger::toString(this) +": Memory status (" + Logger::toString(bytes) + "): " +
                  Logger::toString(res) + " bytes.");
    }

    unsigned int StatsManager::getMemory()
    {
        return Atomic::add(&this->_memory,(unsigned int)0);
    }

    api::uint32_t StatsManager::getUptime()
    {
        return this->_timer.getElapsedTime();
    }

    void StatsManager::addBuffer()
    {
        Atomic::increment(&this->_buffers);
    }

    void StatsManager::removeBuffer()
    {
        Atomic::decrement(&this->_buffers);
    }

    unsigned int StatsManager::getBuffers()
    {
        return this->_buffers;
    }

    void StatsManager::addRequest()
    {
        Atomic::increment(&this->_requests);
    }

    unsigned int StatsManager::getRequests()
    {
        return Atomic::readAndClear(&this->_requests);
    }

    void StatsManager::addSentBytes(unsigned int bytes)
    {
        Atomic::add(&this->_sentBytes, bytes);
    }

    unsigned int StatsManager::getSentBytes()
    {
        return Atomic::readAndClear(&this->_sentBytes);
    }

    void StatsManager::addReceivedBytes(unsigned int bytes)
    {
        Atomic::add(&this->_receivedBytes, bytes);
    }

    unsigned int StatsManager::getReceivedBytes()
    {
        return Atomic::readAndClear(&this->_receivedBytes);
    }
}

