
#ifdef _WIN32
#ifndef __MUTEXWINDOWS_HPP__
# define __MUTEXWINDOWS_HPP__

# include <Windows.h>

# include "utils/NonCopyable.hpp"
# include "Mutex.hpp"

namespace zhttpd
{
    namespace implementation
    {
        class Mutex : private NonCopyable
        {
        private:
            bool _locked;
            HANDLE _mutex;

        public:
            Mutex() : _locked(false)
            {
                this->_mutex = CreateMutex(NULL, false, NULL);
            }

            ~Mutex()
            {
                CloseHandle(this->_mutex);
            }

            void lock()
            {
                WaitForSingleObject(this->_mutex, INFINITE);
                this->_locked = true;
            }

            bool tryLock()
            {
                if (this->_locked)
                    return false;
                if (WaitForSingleObject(this->_mutex, INFINITE) == WAIT_ABANDONED)
                    return false;
                this->_locked = true;
                return true;
            }

            void unlock()
            {
                this->_locked = false;
                ReleaseMutex(this->_mutex);
            }
        };
    }
}

#endif // !__MUTEXWINDOWS_HPP__
#endif // _WIN32
