
#ifndef __THREAD_POOL_HPP__
# define __THREAD_POOL_HPP__

# include <iostream>
# include <vector>
# include <queue>

# include "utils/NonCopyable.hpp"
# include "utils/SmartPtr.hpp"

# include "Thread.hpp"
# include "ThreadCondition.hpp"
# include "Mutex.hpp"
# include "ScopeLock.hpp"
# include "ICondition.hpp"

namespace ZHTTPD
{
    class ThreadPool : private NonCopyable
    {
    private:

        /**
         *
         */
        class ThreadTask : public ITask
        {
        private:
            ThreadPool* _pool;

        public:
            ThreadTask(ThreadPool* pool);
            virtual void run();
        };

        /**
         *
         */
        class CheckHasWork : public ICondition
        {
        private:
            ThreadPool* _pool;

        public:
            CheckHasWork(ThreadPool* pool);
            virtual bool checkCondition();
        };

    private:
        bool _running;
        std::vector<Thread*> _threads;
        std::queue<SmartPtr<ITask> > _pending_tasks;
        ThreadTask _running_thread_task;
        CheckHasWork _checking_has_work;
        ThreadCondition _condition;
        Mutex _pending_tasks_mutex;

    public:
        ThreadPool(unsigned int nb_threads = 8);
        ~ThreadPool();
        void queueTask(SmartPtr<ITask> const& task);
    };


}

#endif // !__THREAD_POOL_HPP__
