
#ifndef __THREAD_HPP__
# define __THREAD_HPP__

//    class IThread
//    {
//    public:
//        virtual ~IThread() {}
//        virtual void join() = 0;
//        virtual void quit() = 0;
//    };

# ifdef _WIN32
#  include "ThreadWindows.hpp"
# else
#  include "ThreadUnix.hpp"
# endif // !_WIN32

namespace zhttpd
{
    typedef implementation::Thread      Thread;
}

#endif // !__THREAD_HPP__
