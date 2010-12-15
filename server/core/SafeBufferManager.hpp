
#ifndef __SAFEBUFFERMANAGER_HPP__
# define __SAFEBUFFERMANAGER_HPP__

# include <set>

# include "api/IBufferManager.hpp"
# include "thread/Mutex.hpp"
# include "thread/ScopeLock.hpp"
# include "utils/Logger.hpp"

namespace zhttpd
{
    class SafeBufferManager : public api::IBufferManager
    {
        typedef std::set<api::IBuffer*> buffers_t;

    private:
        api::IBufferManager&    _manager;
        buffers_t               _buffers;
        Mutex                   _mutex;

    public:
        SafeBufferManager(api::IBufferManager& manager) : _manager(manager) {}
        virtual ~SafeBufferManager()
        {
            if (this->_buffers.size() > 0)
            {
                LOG_WARN("Not all buffers have been released : " +
                         Logger::toString(this->_buffers.size()) +
                         " left");
                buffers_t::iterator it = this->_buffers.begin();
                buffers_t::iterator end = this->_buffers.end();
                for (; it != end; ++it)
                    this->_manager.release(*it);
            }
        }

        virtual api::IBuffer* allocate(std::string const& string)
        {
            if (this->_mutex.tryLock() == false)
            {
                LOG_WARN("Concurrent access detected");
                this->_mutex.lock();
            }
            api::IBuffer* res = this->_manager.allocate(string);
            this->_buffers.insert(res);
            this->_mutex.unlock();
            return res;
        }

        virtual api::IBuffer* allocate(char const* data, size_t size)
        {
            if (this->_mutex.tryLock() == false)
            {
                LOG_WARN("Concurrent access detected");
                this->_mutex.lock();
            }
            api::IBuffer* res = this->_manager.allocate(data, size);
            this->_buffers.insert(res);
            this->_mutex.unlock();
            return res;
        }

        virtual api::IBuffer* allocate(size_t size)
        {
            if (this->_mutex.tryLock() == false)
            {
                LOG_WARN("Concurrent access detected");
                this->_mutex.lock();
            }
            api::IBuffer* res = this->_manager.allocate(size);
            this->_buffers.insert(res);
            this->_mutex.unlock();
            return res;
        }

        virtual void release(api::IBuffer* buffer)
        {
            if (this->_mutex.tryLock() == false)
            {
                LOG_WARN("Concurrent access detected");
                this->_mutex.lock();
            }
            this->_buffers.erase(buffer);
            this->_manager.release(buffer);
            this->_mutex.unlock();
        }

        api::IBufferManager& getBufferManager()
        {
            return this->_manager;
        }
    };
}

#endif /* !__SAFEBUFFERMANAGER_HPP__ */

