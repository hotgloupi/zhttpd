
#ifndef __BUFFERMANAGER_HPP__
# define __BUFFERMANAGER_HPP__

# include "api/IBuffer.hpp"
# include "api/IBufferManager.hpp"
# include "utils/BufferAllocator.hpp"
# include "utils/MemoryPool.hpp"
# include "thread/Mutex.hpp"

# include "Buffer.hpp"

namespace zhttpd
{
    class BufferManager : public api::IBufferManager
    {
        typedef Buffer<BufferAllocator> buffer_t;

        private:
            Mutex _mutex;
            MemoryPool<buffer_t> _pool;
            BufferAllocator _allocator;

        public:
            BufferManager() : _pool(100), _allocator(1000)
            {
            }

            virtual ~BufferManager()
            {
            }

            virtual api::IBuffer* allocate(std::string const& string)
            {
                ZHTTPD_ASSERT_LOCK(this->_mutex);
                char const* str = string.c_str();
                api::size_t size = string.size();
                api::IBuffer* ret = this->_pool.allocate(this->_allocator, str, size);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return ret;
            }

            virtual api::IBuffer* allocate(char const* data, api::size_t size)
            {
                ZHTTPD_ASSERT_LOCK(this->_mutex);
                api::IBuffer* ret = this->_pool.allocate(this->_allocator, data, size);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return ret;
            }

            virtual api::IBuffer* allocate(api::size_t size)
            {
                ZHTTPD_ASSERT_LOCK(this->_mutex);
                api::IBuffer* ret = this->_pool.allocate(this->_allocator, size);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return ret;
            }

            virtual void release(api::IBuffer* buffer)
            {
                ZHTTPD_ASSERT_LOCK(this->_mutex);
#ifdef ZHTTPD_DEBUG
                buffer_t* b = dynamic_cast<buffer_t*>(buffer);
                assert(b != 0);
                this->_pool.release(b);
#else
                this->_pool.release(reinterpret_cast<buffer_t*>(buffer));
#endif
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
            }
    };
}

#endif /* !__BUFFERMANAGER_HPP__ */

