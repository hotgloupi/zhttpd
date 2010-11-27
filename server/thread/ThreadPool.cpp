
# include <vector>
# include <queue>

# include "utils/NonCopyable.hpp"
# include "utils/SmartPtr.hpp"
# include "utils/Logger.hpp"

# include "ThreadPool.hpp"
# include "ThreadCondition.hpp"
# include "Mutex.hpp"
# include "ScopeLock.hpp"
# include "ICondition.hpp"

using namespace zhttpd;

ThreadPool::ThreadTask::ThreadTask(ThreadPool* pool) : _pool(pool)
{
}

void ThreadPool::ThreadTask::run()
{
    while (this->_pool->_running)
    {
        try
        {
            this->_pool->_condition.wait();
            if (!this->_pool->_running)
                break;

            SmartPtr<ITask> work;
            {
                ScopeLock sl(this->_pool->_pending_tasks_mutex);
                if (this->_pool->_pending_tasks.empty())
                    continue;
                work = this->_pool->_pending_tasks.front();
                this->_pool->_pending_tasks.pop();
            }
            work->run();
        }
        catch (std::exception& ex)
        {
            LOG_ERROR("Unhandled exception: " + ex.what());
        }
    }
    LOG_DEBUG("Thread from thread pool exit !");
}

ThreadPool::CheckHasWork::CheckHasWork(ThreadPool* pool) : _pool(pool)
{
}

bool ThreadPool::CheckHasWork::checkCondition()
{
    if (!this->_pool->_running)
        return true;
    ScopeLock sl(this->_pool->_pending_tasks_mutex);
    return !(this->_pool->_pending_tasks.empty());
}

ThreadPool::ThreadPool(unsigned int nb_threads) :
    _running(true),
    _threads(nb_threads),
    _pending_tasks(),
    _running_thread_task(this),
    _checking_has_work(this),
    _condition(&_checking_has_work),
    _pending_tasks_mutex()
{
    for (unsigned int i = 0; i < nb_threads; ++i)
        this->_threads[i] = new Thread(&this->_running_thread_task);
}

ThreadPool::~ThreadPool()
{
    if (!this->_running)
        return;
    this->_running = false;
    this->_condition.wakeAll();
    std::vector<Thread*>::iterator it = this->_threads.begin();
    std::vector<Thread*>::iterator ite = this->_threads.end();
    for (; it != ite; ++it)
    {
        (*it)->join();
        delete *it;
    }
}

void ThreadPool::queueTask(SmartPtr<ITask> const& task)
{
    {
        ScopeLock sl(this->_pending_tasks_mutex);
        this->_pending_tasks.push(task);
    }
    this->_condition.wakeAll();
}
