#ifndef _WIN32
# ifndef __THREADUNIX_HPP__
#  define __THREADUNIX_HPP__

#  include <pthread.h>
#  include <stdexcept>

#  include "thread/ITask.hpp"
#  include "utils/NonCopyable.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class Thread : private NonCopyable
        {
        private:
            typedef void* (*pthread_run_t)(void*);
            pthread_t* _thread;
            bool _running;
            ITask* _callback_instance;

            static void* _run(Thread* thread);
        public:
            Thread(ITask* instance);
            virtual ~Thread();
            void join();
            void quit();
        };
    }
}

# endif // !__THREADUNIX_HPP__
#endif // !_WIN32
