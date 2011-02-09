
#ifndef __REQUESTTASKS_HPP__
# define __REQUESTTASKS_HPP__

# include <list>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "utils/Timer.hpp"
# include "core/ModuleWrapper.hpp"

namespace zhttpd
{
    class Request;
    class ModuleTask;

    class RequestTasks : public ISocketEventHandler
    {
    public:
        typedef boost::function<bool(api::event::Type, Request&, ModuleWrapper&, api::IBuffer*)> task_t;

    private:
        typedef std::list<task_t> tasks_t;
        typedef std::list<api::IBuffer*> buffers_t;


    private:
        Request&        _request;
        volatile bool   _can_read;
        volatile bool   _can_write;
        volatile bool   _has_error;
        ModuleTask*     _call_later_task;
        api::uint32_t   _call_later_time;
        Timer           _call_later_timer;
        bool            _wait_read;
        bool            _wait_write;
        bool            _registered;
        ModuleTask*     _end_task;
        buffers_t       _write_buffers;
        tasks_t         _tasks;
        Timer           _timer;
        Mutex           _mutex;

    public:
        RequestTasks(Request& request);
        ~RequestTasks();

        // non-concurrent (from the request)
        SmartPtr<ModuleTask> getNextTask();
        bool hasWork();
        bool hasPendingEvent();
        bool hasError();
        void addTask(api::event::Type event,
                     Request& request,
                     ModuleWrapper& module,
                     api::IBuffer* buffer);
        void needRead();
        void needWrite(api::IBuffer* buffer);
        void reset();

        // concurrent (from the request manager)
        void notifyHasError();
        void handleSocketEvent(socket_event::Type evt);
        void addEndTask(Request& request, ModuleWrapper& module);
        void addCallLaterTask(Request& request, ModuleWrapper& module, api::uint32_t delay);

    private:
        void _register();
        void _unregister();

        void _addTask(api::event::Type event,
                     Request& request,
                     ModuleWrapper& module,
                     api::IBuffer* buffer);
    };
}

#endif /* !__REQUESTTASKS_HPP__ */

