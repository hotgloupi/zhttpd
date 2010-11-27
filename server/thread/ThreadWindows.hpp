
#ifdef _WIN32
# ifndef __THREADWINDOWS_HPP__
#  define __THREADWINDOWS_HPP__

#  include <Windows.h>
#  include <stdexcept>

#  include "thread/ITask.hpp"
#  include "utils/NonCopyable.hpp"
#  include "utils/String.hpp"

namespace zhttpd
{
    namespace implementation
    {
        class Thread : private NonCopyable
        {
        private:
            HANDLE _thread;
            bool _running;
            ITask* _callback_instance;

            static DWORD _run(Thread* thread);
        public:
            Thread(ITask* instance);
            ~Thread();
            void join();
            void quit();
        };
    }
}

# endif // !__THREADWINDOWS_HPP__
#endif // _WIN32
