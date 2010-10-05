
#ifndef __MUTEX_HPP__
# define __MUTEX_HPP__

//    class IMutex
//    {
//    public:
//        virtual ~IMutex() {}
//        virtual void lock() = 0;
//        virtual bool tryLock() = 0;
//        virtual void unlock() = 0;
//    };

# ifdef _WIN32
#  include "MutexWindows.hpp"
# else // !_WIN32
#  include "MutexUnix.hpp"
# endif

namespace ZHTTPD
{
    typedef IMPLEMENTATION::Mutex       Mutex;
}

#endif // !__MUTEX_HPP__
