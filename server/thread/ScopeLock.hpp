
#ifndef __SCOPELOCK_HPP__
# define __SCOPELOCK_HPP__

#include "Mutex.hpp"

namespace zhttpd
{
    class ScopeLock
    {
    private:
        Mutex& _mutex;

    public:
        explicit inline ScopeLock(Mutex& mutex) : _mutex(mutex)
        {
            this->_mutex.lock();
        }

        inline ~ScopeLock(void)
        {
            this->_mutex.unlock();
        }

    };
}

#endif // !__SCOPELOCK_HPP__
