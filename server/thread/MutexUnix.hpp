
#ifndef _WIN32
#ifndef __MUTEXUNIX_HPP__
# define __MUTEXUNIX_HPP__

# include <cassert>
# include <pthread.h>
# include <stdexcept>

# include "utils/NonCopyable.hpp"
# include "utils/Logger.hpp"

# include "Mutex.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class Mutex : private NonCopyable
        {
        private:
            bool _locked;
            pthread_mutex_t _mutex;

        public:
            Mutex() : _locked(false)
            {
                if (::pthread_mutex_init(&this->_mutex, NULL) != 0)
                    throw std::runtime_error("pthread_mutex_init fail");
            }

            ~Mutex()
            {
                if (::pthread_mutex_destroy(&this->_mutex) != 0)
                    throw std::runtime_error(std::string("pthread_mutex_destroy() failure (locked=") + Logger::toString<bool>(this->_locked) + ")");
            }

            void lock()
            {
                if (::pthread_mutex_lock(&this->_mutex) != 0)
                    throw std::runtime_error("pthread_mutex_lock() failure");
                this->_locked = true;
            }

            bool tryLock()
            {
                if (::pthread_mutex_trylock(&this->_mutex) != 0)
                    return false;
                this->_locked = true;
                return true;
            }

            void unlock()
            {
                if (::pthread_mutex_unlock(&this->_mutex) != 0)
                    throw std::runtime_error("pthread_mutex_unlock() failure");
                this->_locked = false;
            }
        };
    }
}

# endif // !__MUTEXUNIX_HPP__
#endif // !_WIN32
