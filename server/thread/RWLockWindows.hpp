#ifdef _WIN32
#ifndef __RWLOCKWINDOWS_HPP__
# define __RWLOCKWINDOWS_HPP__

# include <windows.h>

# include "utils/NonCopyable.hpp"

namespace ZHTTPD
{
    class RWLock : private NonCopyable
    {
    private:
        SRWLOCK _rwlock;

    public:
        RWLock()
        {
            InitializeSRWLock(&this->_rwlock);
        }

        ~RWLock() {}

        void lockRead()
        {
            AcquireSRWLockShared(&this->_rwlock);
        }

        void lockWrite()
        {
            AcquireSRWLockExclusive(&this->_rwlock);
        }

        bool tryLockRead()
        {
            return TryAcquireSRWLockShared(&this->_rwlock) != 0;
        }

        bool tryLockWrite()
        {
            return TryAcquireSRWLockExclusive(&this->_rwlock) != 0;
        }

        void unlockRead()
        {
            ReleaseSRWLockShared(&this->_rwlock);
        }

        void unlockWrite()
        {
            ReleaseSRWLockExclusive(&this->_rwlock);
        }
    };
}

#endif // !__RWLOCKWINDOWS_HPP__
#endif // _WIN32
