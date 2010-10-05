
#ifdef _WIN32
#ifndef __THREADCONDITIONWINDOWS_HPP__
# define __THREADCONDITIONWINDOWS_HPP__

# include <Windows.h>

# include "utils/NonCopyable.hpp"
# include "IThreadCondition.hpp"
# include "ICondition.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class ThreadCondition : public IThreadCondition, private NonCopyable
        {
        private:
            ICondition* _wait_condition;
            CONDITION_VARIABLE _condition;
            CRITICAL_SECTION _critical_section;

        public:
            ThreadCondition(ICondition* wait_condition) : _wait_condition(wait_condition)
            {
                ::InitializeConditionVariable(&this->_condition);
                ::InitializeCriticalSection(&this->_critical_section);
            }

            virtual ~ThreadCondition()
            {
                ::DeleteCriticalSection(&this->_critical_section);
            }

            virtual void wait()
            {
                ::EnterCriticalSection(&this->_critical_section);
                while (!this->_wait_condition->checkCondition())
                    ::SleepConditionVariableCS(&this->_condition, &this->_critical_section, INFINITE);
                ::LeaveCriticalSection(&this->_critical_section);
            }

            virtual void wake()
            {
                ::EnterCriticalSection(&this->_critical_section);
                if (this->_wait_condition->checkCondition())
                    ::WakeConditionVariable(&this->_condition);
                ::LeaveCriticalSection(&this->_critical_section);
            }

            virtual void wakeAll()
            {
                ::WakeAllConditionVariable(&this->_condition);
            }
        };
    }
}

#endif // !__THREADCONDITIONWINDOWS_HPP__
#endif // _WIN32
