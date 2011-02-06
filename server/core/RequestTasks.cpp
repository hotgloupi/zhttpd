
#include "thread/ScopeLock.hpp"

#include "ModuleTask.hpp"
#include "Request.hpp"
#include "RequestTasks.hpp"
#include "RequestManager.hpp"

using namespace zhttpd;

RequestTasks::RequestTasks(Request& request) :
    _request(request),
    _can_read(false),
    _can_write(false),
    _has_error(false),
    _call_later_task(0),
    _call_later_time(0),
    _call_later_timer(),
    _wait_read(false),
    _wait_write(false),
    _registered(false),
    _end_task(0)
{
}

void RequestTasks::addEndTask(Request& request, ModuleWrapper& module)
{
    ZHTTPD_LOCK(this->_mutex);
    if (this->_end_task != 0)
        delete this->_end_task;
    this->_end_task = new ModuleTask(api::event::ON_END, request, module, 0);
    ZHTTPD_UNLOCK(this->_mutex);
}

void RequestTasks::addCallLaterTask(Request& request, ModuleWrapper& module, api::uint32_t delay)
{
    ZHTTPD_LOCK(this->_mutex);
    if (delay == 0)
    {
        this->_tasks.push_back(new ModuleTask(api::event::ON_IDLE, request, module, 0));
        ZHTTPD_UNLOCK(this->_mutex);
        return;
    }
    if (this->_call_later_task != 0)
        this->_tasks.push_back(this->_call_later_task);
    this->_call_later_timer.reset();
    this->_call_later_time = delay;
    this->_call_later_task = new ModuleTask(api::event::ON_IDLE, request, module, 0);
    ZHTTPD_UNLOCK(this->_mutex);
}

void RequestTasks::addTask(api::event::Type event,
                           Request& request,
                           ModuleWrapper& module,
                           api::IBuffer* buffer)
{
    assert(event != api::event::ON_END);
    assert(event != api::event::ON_IDLE);
    ZHTTPD_LOCK(this->_mutex);
    this->_addTask(event, request, module, buffer);
    ZHTTPD_UNLOCK(this->_mutex);
}


void RequestTasks::_addTask(api::event::Type event,
                           Request& request,
                           ModuleWrapper& module,
                           api::IBuffer* buffer)
{
    if (event == api::event::ON_CAN_WRITE ||
        event == api::event::ON_RESPONSE_DATA ||
        event == api::event::ON_REQUEST_DATA)
        this->_tasks.push_front(new ModuleTask(event, request, module, buffer));
    else
        this->_tasks.push_back(new ModuleTask(event, request, module, buffer));
}



RequestTasks::~RequestTasks()
{
#ifdef ZHTTPD_DEBUG
    if ((this->_tasks.size() > 0 || this->_write_buffers.size() > 0) && !this->_has_error)
    {
        LOG_WARN("Dumping RequestTasks:");
        LOG_WARN("_can_read = " + Logger::toString( _can_read));
        LOG_WARN("_can_write = " + Logger::toString(_can_write));
        LOG_WARN("_has_error = " + Logger::toString(_has_error));
        LOG_WARN("_call_later_task ? " + Logger::toString(_call_later_task != 0 ? "yes" : "no"));
        LOG_WARN("_wait_read = " + Logger::toString(_wait_read));
        LOG_WARN("_wait_write = " + Logger::toString(_wait_write));
        LOG_WARN("_registered = " + Logger::toString(_registered));
        LOG_WARN("_end_task ? " + Logger::toString(_end_task != 0 ? "yes" : "no"));
        LOG_WARN("wbuf size = " + Logger::toString(_write_buffers.size()));
        LOG_WARN("tasks size = " + Logger::toString(_tasks.size()));
    }
#endif
    this->reset();
}

SmartPtr<ModuleTask> RequestTasks::getNextTask()
{
    ZHTTPD_LOCK(this->_mutex);
    SmartPtr<ModuleTask> task = this->_tasks.front();
    this->_tasks.pop_front();
    ZHTTPD_UNLOCK(this->_mutex);
    return task;
}

bool RequestTasks::hasWork()
{
    ZHTTPD_LOCK(this->_mutex);
    if (this->_has_error)
    {
        ZHTTPD_UNLOCK(this->_mutex);
        return false;
    }
    if ((this->_wait_read || this->_wait_write) &&
        (!this->_wait_read || (this->_wait_read && !this->_can_read)) &&
        (!this->_wait_write || (this->_wait_write && !this->_can_write)) &&
        (this->_timer.getElapsedTime() > 5000))
    {
        LOG_WARN("Request have reached timeout "+Logger::toString(&this->_request));
        this->_wait_read = false;
        this->_wait_write = false;
        this->_has_error = true;
        this->_unregister();
        ZHTTPD_UNLOCK(this->_mutex);
        return false;
    }

    if (this->_wait_read && this->_can_read)
    {
        this->_wait_read = false;
        this->_can_read = false;
        this->_addTask(api::event::ON_CAN_READ,
                      this->_request,
                      *this->_request.getInputModule(),
                      0);
    }
    if (this->_wait_write && (this->_can_write || this->_write_buffers.size() > 10))
    {
        assert(this->_write_buffers.size() > 0);
        LOG_DEBUG("pending buffers " + Logger::toString(this->_write_buffers.size()));
        this->_can_write = false;
        this->_wait_write = false;
        api::IBuffer* buf = this->_write_buffers.front();
        this->_write_buffers.pop_front();
        this->_addTask(api::event::ON_CAN_WRITE,
                      this->_request,
                      *this->_request.getOutputModule(),
                      buf);
        this->_wait_write = this->_write_buffers.size() > 0;
        this->_register();
    }
    if (this->_call_later_task != 0 &&
        this->_call_later_timer.getElapsedTime() > this->_call_later_time)
    {
        this->_tasks.push_back(this->_call_later_task);
        this->_call_later_task = 0;
    }
    if (this->_end_task != 0 && this->_tasks.size() == 0 && this->_write_buffers.size() == 0)
    {
        this->_tasks.push_back(this->_end_task);
        this->_end_task = 0;
    }
    bool res = this->_tasks.size() > 0;
    ZHTTPD_UNLOCK(this->_mutex);
    return res;
}

bool RequestTasks::hasPendingEvent()
{
    ZHTTPD_LOCK(this->_mutex);
    bool res = !this->_has_error && (
        (this->_call_later_task != 0) ||
        this->_registered ||
        this->_wait_read ||
        this->_wait_write
    );
    ZHTTPD_UNLOCK(this->_mutex);
    return res;
}

bool RequestTasks::hasError()
{
    return this->_has_error;
}

void RequestTasks::needRead()
{
    ZHTTPD_LOCK(this->_mutex);
    if (!this->_wait_read)
    {
        this->_wait_read = true;
        this->_register();
    }
    ZHTTPD_UNLOCK(this->_mutex);
}

void RequestTasks::needWrite(api::IBuffer* buffer)
{
    ZHTTPD_LOCK(this->_mutex);
    this->_write_buffers.push_back(buffer);
    if (!this->_wait_write)
    {
        this->_wait_write = true;
        this->_register();
    }
    ZHTTPD_UNLOCK(this->_mutex);
}


void RequestTasks::notifyHasError()
{
    ZHTTPD_LOCK(this->_mutex);
    this->_has_error = true;
    this->_unregister();
    ZHTTPD_UNLOCK(this->_mutex);
}

void RequestTasks::handleSocketEvent(socket_event::Type evt)
{

    ZHTTPD_LOCK(this->_mutex);
    if (this->_has_error == true)
    {
        ZHTTPD_UNLOCK(this->_mutex);
        return ;
    }
    switch (evt)
    {
    case socket_event::CAN_READ:
        if (this->_can_read)
        {
            ZHTTPD_UNLOCK(this->_mutex);
            return;
        }
        this->_can_read = true;
        this->_unregister();
        this->_timer.reset();
        break;
    case socket_event::CAN_WRITE:
        if (this->_can_write)
        {
            ZHTTPD_UNLOCK(this->_mutex);
            return;
        }
        this->_can_write = true;
        this->_unregister();
        this->_timer.reset();
        break;
    case socket_event::HAS_ERROR:
        this->_can_read = false;
        this->_can_write = false;
        this->_has_error = true;
        this->_unregister();
        break;
    default:
        LOG_ERROR("Unexpected socket event type !");
    }
    ZHTTPD_UNLOCK(this->_mutex);
}


void RequestTasks::_register()
{
    if (!this->_registered)
    {
        if (((this->_wait_read && !this->_can_read) ||
             (this->_wait_write && !this->_can_write)) &&
            !this->_has_error)
        {
            this->_registered = true;
//            RequestManager::getInstance()->monitorSocketEvent(this->_request);
        }
    }
}

void RequestTasks::_unregister()
{
    if (this->_registered)
    {
        if (((this->_can_read || !this->_wait_read)
             && (this->_can_write || !this->_wait_write))
            || this->_has_error)
        {
            this->_registered = false;
//            RequestManager::getInstance()->ignoreSocketEvent(this->_request);
        }
    }
}

void RequestTasks::reset()
{
    ZHTTPD_LOCK(this->_mutex);
    this->_wait_read = false;
    this->_wait_write = false;
    this->_unregister();
    tasks_t::iterator it = this->_tasks.begin();
    tasks_t::iterator end = this->_tasks.end();

    for (; it != end; ++it)
    {
        delete *it;
    }
    ZHTTPD_DELETE(this->_call_later_task);
    ZHTTPD_DELETE(this->_end_task);
    buffers_t::iterator it_buf = this->_write_buffers.begin();
    buffers_t::iterator ite_buf = this->_write_buffers.end();
    for (; it_buf != ite_buf; ++it_buf)
    {
        this->_request.getBufferManager().release(*it_buf);
    }
    this->_tasks.clear();
    ZHTTPD_UNLOCK(this->_mutex);
}
