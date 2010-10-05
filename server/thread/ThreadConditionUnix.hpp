
#ifndef _WIN32
# ifndef __THREADCONDITIONUNIX_HPP__
#  define __THREADCONDITIONUNIX_HPP__

#  include <pthread.h>
#  include <stdexcept>

#  include "utils/NonCopyable.hpp"

#  include "IThreadCondition.hpp"
#  include "ICondition.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class ThreadCondition : public IThreadCondition, private NonCopyable
        {
        private:
            ICondition* _wait_condition;
            pthread_cond_t _condition;
            pthread_mutex_t _mutex;

        public:
            ThreadCondition(ICondition* wait_condition) : _wait_condition(wait_condition)
            {
                ::pthread_cond_init(&this->_condition, 0);
                ::pthread_mutex_init(&this->_mutex, 0);
            }

            virtual ~ThreadCondition()
            {
                ::pthread_mutex_lock(&this->_mutex);
                ::pthread_cond_broadcast(&this->_condition);
                ::pthread_mutex_unlock(&this->_mutex);

                ::pthread_cond_destroy(&this->_condition);
                if (::pthread_mutex_destroy(&this->_mutex) != 0)
                    throw std::runtime_error("pthread_mutex_destroy() failure");
            }

            virtual void wait()
            {
                while (!this->_wait_condition->checkCondition())
                {
                    ::pthread_mutex_lock(&this->_mutex);
                    ::pthread_cond_wait(&this->_condition, &this->_mutex);
                    ::pthread_mutex_unlock(&this->_mutex);
                }
            }

            virtual void wake()
            {
                if (this->_wait_condition->checkCondition())
                {
                    ::pthread_mutex_lock(&this->_mutex);
                    ::pthread_cond_signal(&this->_condition);
                    ::pthread_mutex_unlock(&this->_mutex);
                }
            }

            virtual void wakeAll()
            {
                ::pthread_cond_broadcast(&this->_condition);
            }
        };
    }
}

# endif // !__THREADCONDITIONUNIX_HPP__
#endif // !_WIN32
