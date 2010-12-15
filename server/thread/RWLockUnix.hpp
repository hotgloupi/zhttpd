#ifndef _WIN32
#ifndef __RWLOCKUNIX_HPP__
# define __RWLOCKUNIX_HPP__

# include <stdexcept>
# include <pthread.h>
# include <errno.h>

# include "utils/NonCopyable.hpp"

namespace zhttpd
{
    class RWLock : private NonCopyable
    {
    private:
        pthread_rwlock_t _rwlock;

    public:
        RWLock()
        {
            if (::pthread_rwlock_init(&this->_rwlock, 0) != 0)
                throw std::runtime_error("pthread_rwlock_init");
        }

        ~RWLock()
        {
            if (::pthread_rwlock_destroy(&this->_rwlock) != 0)
                throw std::runtime_error("pthread_rwlock_destroy");
        }

        void lockRead()
        {
            if (::pthread_rwlock_rdlock(&this->_rwlock) != 0)
                throw std::runtime_error("pthread_rwlock_rdlock");
        }

        void lockWrite()
        {
            if (::pthread_rwlock_wrlock(&this->_rwlock) != 0)
                throw std::runtime_error("pthread_rwlock_wrlock");
        }

        bool tryLockRead()
        {
            int res = ::pthread_rwlock_tryrdlock(&this->_rwlock);
            if (res != 0)
            {
                if (res != EBUSY)
                    throw std::runtime_error("pthread_rwlock_tryrdlock");
                return false;
            }
            return true;
        }

        bool tryLockWrite()
        {
            int res = ::pthread_rwlock_trywrlock(&this->_rwlock);
            if (res != 0)
            {
                if (res != EBUSY)
                    throw std::runtime_error("pthread_rwlock_trywrlock");
                return false;
            }
            return true;
        }

        void unlockRead()
        {
            if (::pthread_rwlock_unlock(&this->_rwlock) != 0)
                throw std::runtime_error("pthread_rwlock_unlock");
        }

        void unlockWrite()
        {
            if (::pthread_rwlock_unlock(&this->_rwlock) != 0)
                throw std::runtime_error("pthread_rwlock_unlock");
        }
    };
}

#endif // !__RWLOCKUNIX_HPP__
#endif // !_WIN32
