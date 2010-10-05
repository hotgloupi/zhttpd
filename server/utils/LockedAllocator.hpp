

#ifndef __LOCKEDALLOCATOR_HPP__
# define __LOCKEDALLOCATOR_HPP__

# include "thread/Mutex.hpp"
# include "utils/macros.hpp"
# include "utils/MemoryPool.hpp"

namespace ZHTTPD
{
    template<typename Class>
    class LockedAllocator
    {
    private:
        MemoryPool<Class>   _pool;
        Mutex               _mutex;

    public:

        virtual ~LockedAllocator() {}

        Class* allocate()
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = this->_pool.allocate();
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        template<typename T1>
        Class* allocate(T1& p1)
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = this->_pool.allocate(p1);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        template<typename T1, typename T2>
        Class* allocate(T1& p1, T2& p2)
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = this->_pool.allocate(p1, p2);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        template<typename T1, typename T2, typename T3>
        Class* allocate(T1& p1, T2& p2, T3& p3)
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = this->_pool.allocate(p1, p2, p3);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        void release(Class* obj)
        {
            ZHTTPD_LOCK(this->_mutex);
            this->_pool.release(obj);
            ZHTTPD_UNLOCK(this->_mutex);
        }
    };
}

#endif /* !__LOCKEDALLOCATOR_HPP__ */

