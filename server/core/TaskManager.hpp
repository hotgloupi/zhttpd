
#ifndef __TASKMANAGER_HPP__
# define __TASKMANAGER_HPP__

# include <list>
# include <set>

# include "thread/ThreadPool.hpp"
# include "thread/Mutex.hpp"
# include "thread/ITask.hpp"
# include "utils/Singleton.hpp"

# include "Request.hpp"

namespace zhttpd
{
    class TaskManager :
        public Singleton<TaskManager>,
        public ITask
    {
        friend class Singleton<TaskManager>;
        typedef std::list<Request*> request_list_t;
        typedef std::set<Request*> request_set_t;

    private:
        ThreadPool      _thread_pool;
        request_set_t   _requests;
        request_set_t   _pending_add_requests;
        Mutex           _pending_add_requests_mutex;
        request_set_t   _pending_del_requests;

    public:
        void notifyEndTask(Request& request);
        void startRequest(Request& request);
        void stopRequest(Request& request);
        virtual void run();

    private:
        TaskManager();
        virtual ~TaskManager();
        api::size_t _giveWork();
        api::size_t _addPendingRequests();
        api::size_t _delPendingRequests();
    };
}

#endif /* !__TASKMANAGER_HPP__ */

