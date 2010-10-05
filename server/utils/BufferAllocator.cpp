#include "BufferAllocator.hpp"
#include "utils/Logger.hpp"
#include "utils/StatsManager.hpp"

#include <cstring> // memcpy
#include <algorithm> // find

namespace ZHTTPD
{
    BufferAllocator::BufferAllocator(API::size_t size)
    {
        LOG_DEBUG("CREATE BufferAllocator " + Logger::toString(this));
        this->_free_blocks.reserve(size);
        this->_blocks.reserve(size);
    }

    BufferAllocator::~BufferAllocator()
    {
        LOG_DEBUG("DELETE  BufferAllocator " + Logger::toString(this));
        this->clean();
        if (!this->_blocks.empty())
            LOG_WARN("There are blocks left (all buffers were not released).");
    }

    void BufferAllocator::clean()
    {
        unsigned int count = 0;
        std::vector<char*>::reverse_iterator it = this->_blocks.rbegin();
        std::vector<char*>::reverse_iterator itEnd = this->_blocks.rend();
        while (it != itEnd)
        {
            std::vector<char*>::reverse_iterator f = std::find(this->_free_blocks.rbegin(), this->_free_blocks.rend(), *it);
            if (f != this->_free_blocks.rend())
            {
                ++count;
                delete [] *it;
                this->_blocks.erase(it.base() - 1);
                it++;
                this->_free_blocks.erase(f.base() - 1);
            }
            else
                ++it;
        }
        if (!this->_free_blocks.empty())
        {
            LOG_WARN("Double free detected...");
            this->_free_blocks.clear();
        }
        StatsManager::getInstance()->addMemory(-count * BLOCK_SIZE);
        LOG_INFO("Deleted " + Logger::toString(count) + " blocks.");
    }

    char* BufferAllocator::allocate(API::size_t size)
    {
        if (size > BLOCK_SIZE)
        {
            StatsManager::getInstance()->addMemory(size);
            return new char[size];
        }
        if (this->_free_blocks.empty())
        {
            char* n = new char[BLOCK_SIZE];
            this->_blocks.push_back(n);
            LOG_DEBUG("New one !");
            StatsManager::getInstance()->addMemory(BLOCK_SIZE);
#ifdef ZHTTPD_DEBUG
            this->_debug[n] = size;
#endif
            return n;
        }
        char* ret = this->_free_blocks.back();
        this->_free_blocks.pop_back();
#ifdef ZHTTPD_DEBUG
        this->_debug[ret] = size;
#endif
        return ret;
    }

    char* BufferAllocator::resize(char* data, API::size_t old_size, API::size_t new_size)
    {
#ifdef ZHTTPD_DEBUG
        if (this->_debug.count(data) == 0)
            LOG_ERROR("Resizing an unknown buffer.");
        else if (this->_debug[data] != old_size)
            LOG_ERROR("Bad old size given.");
        else
            this->_debug[data] = new_size;
#endif
        if (new_size <= old_size)
            return data;
        if (old_size > BLOCK_SIZE)
        {
            StatsManager::getInstance()->addMemory(new_size - old_size);
            char* ret = new char[new_size];
            ::memcpy(ret, data, old_size);
#ifdef ZHTTPD_DEBUG
            this->_debug.erase(data);
            this->_debug[ret] = new_size;
#endif
            delete [] data;
            return ret;
        }
        if (new_size > BLOCK_SIZE)
        {
            StatsManager::getInstance()->addMemory(new_size);
            char* ret = new char[new_size];
            ::memcpy(ret, data, old_size);
#ifdef ZHTTPD_DEBUG
            this->_debug.erase(data);
            this->_debug[ret] = new_size;
#endif
            this->release(data, old_size);
            return ret;
        }
        return data;
    }

    void BufferAllocator::release(char* data, API::size_t size)
    {
#ifdef ZHTTPD_DEBUG
        if (this->_debug.count(data) == 0)
            LOG_ERROR("Releasing an unknown buffer.");
        else
            this->_debug.erase(data);
#endif
        if (size > BLOCK_SIZE)
        {
            delete [] data;
            StatsManager::getInstance()->addMemory(-size);
            return;
        }
        this->_free_blocks.push_back(data);
    }

}

