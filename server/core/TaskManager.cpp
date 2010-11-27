
#include "utils/Timer.hpp"
#include "utils/Logger.hpp"
#include "utils/SmartPtr.hpp"
#include "utils/macros.hpp"

#include "ModuleTask.hpp"
#include "TaskManager.hpp"
#include "ServerState.hpp"
#include "RequestManager.hpp"
#include "SessionManager.hpp"

using namespace zhttpd;

TaskManager::TaskManager() : _thread_pool(4)
{
}

TaskManager::~TaskManager()
{
    LOG_INFO("Pending add: " +Logger::toString(this->_pending_add_requests.size()));
    LOG_INFO("Pending del: " +Logger::toString(this->_pending_del_requests.size()));
    LOG_INFO("Current: " +Logger::toString(this->_requests.size()));
    unsigned int attempts = 0;
    while (true)
    {
        api::size_t left_task = 0;

        request_set_t::iterator it = this->_requests.begin();
        request_set_t::iterator end = this->_requests.end();

        for (; it != end; ++it)
        {
            if ((*it)->isQueued())
                left_task++;
        }
        if (left_task == 0 || attempts > 10)
            break ;
        else
            LOG_INFO("Waiting for " + Logger::toString(left_task) + " tasks left");
        ++attempts;
        Timer::sleep(500);
    }
    request_set_t::iterator it = this->_requests.begin();
    request_set_t::iterator end = this->_requests.end();

    for (; it != end; ++it)
        RequestManager::getInstance()->endRequest(*it);
}

void TaskManager::notifyEndTask(Request& request)
{
/*    ZHTTPD_LOCK(this->_being_processed_mutex);
    this->_being_processed.erase(&request);
    ZHTTPD_UNLOCK(this->_being_processed_mutex);
    */
    request.setQueued(false);
}

void TaskManager::startRequest(Request& request)
{
    ZHTTPD_LOCK(this->_pending_add_requests_mutex);
    this->_pending_add_requests.insert(&request);
    ZHTTPD_UNLOCK(this->_pending_add_requests_mutex);
}

api::size_t TaskManager::_giveWork()
{
    request_set_t::iterator it = this->_requests.begin();
    request_set_t::iterator end = this->_requests.end();
    api::size_t count = 0;

    for (; it != end; ++it)
    {
        Request* request = *it;
        if (!request->isQueued())
        {
            RequestTasks& tasks = request->getRequestTasks();
            if (tasks.hasWork())
            {
                request->setQueued(true);
                SmartPtr<ModuleTask> task = request->getRequestTasks().getNextTask();
                this->_thread_pool.queueTask(task);
                ++count;
            }
            else if (!tasks.hasPendingEvent())
            {
                if (!tasks.hasError() && request->getRequestHeader("Connection") != "close")
                {
                    Socket& s = request->getServerSession().popServerSocket();
                    SessionManager::getInstance()->handleNewSession(&s, request->getSession().getPort());
                }
                this->_pending_del_requests.insert(request);
            }
        }
    }
    return count;
}

api::size_t TaskManager::_addPendingRequests()
{
    ZHTTPD_LOCK(this->_pending_add_requests_mutex);
    request_set_t::iterator it = this->_pending_add_requests.begin();
    request_set_t::iterator end = this->_pending_add_requests.end();
    api::size_t count = this->_pending_add_requests.size();

    for (; it != end; ++it)
        this->_requests.insert(*it);
    this->_pending_add_requests.clear();
    ZHTTPD_UNLOCK(this->_pending_add_requests_mutex);
    return count;
}

api::size_t TaskManager::_delPendingRequests()
{
    request_set_t::iterator it = this->_pending_del_requests.begin();
    request_set_t::iterator end = this->_pending_del_requests.end();
    api::size_t count = this->_pending_del_requests.size();
    for (; it != end; ++it)
    {
        this->_requests.erase(*it);
        RequestManager::getInstance()->endRequest(*it);
    }
    this->_pending_del_requests.clear();
    return count;
}

void TaskManager::run()
{
    ServerState* sv = ServerState::getInstance();
    bool should_sleep;

    LOG_INFO("TaskManager is up");
    while (sv->getState() == ServerState::RUNNING)
    {
        should_sleep = (this->_giveWork() == 0);
        should_sleep = (this->_delPendingRequests() == 0) && should_sleep;
        should_sleep = (this->_addPendingRequests() == 0) && should_sleep;
        if (should_sleep)
            Timer::sleep(1);
    }

    LOG_INFO("TaskManager is down");
}

