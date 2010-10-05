
#ifndef __REQUESTTASKS_HPP__
# define __REQUESTTASKS_HPP__

# include <list>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "utils/SmartPtr.hpp"
# include "utils/Timer.hpp"
# include "thread/Mutex.hpp"
# include "socket/ISocketEventHandler.hpp"

# include "ModuleWrapper.hpp"

namespace ZHTTPD
{
    class Request;
    class ModuleTask;

    class RequestTasks : public ISocketEventHandler
    {
    private:
        typedef std::list<ModuleTask*> tasks_t;
        typedef std::list<API::IBuffer*> buffers_t;

    private:
        Request&        _request;
        volatile bool   _can_read;
        volatile bool   _can_write;
        volatile bool   _has_error;
        ModuleTask*     _call_later_task;
        API::uint32_t   _call_later_time;
        Timer           _call_later_timer;
        bool            _wait_read;
        bool            _wait_write;
        bool            _registered;
        ModuleTask*     _end_task;
        buffers_t       _write_buffers;
        tasks_t         _tasks;
        Timer           _timer;
        Mutex           _mutex;

        void _register();
        void _unregister();

        void _addTask(API::EVENT::Type event,
                     Request& request,
                     ModuleWrapper& module,
                     API::IBuffer* buffer);
    public:
        RequestTasks(Request& request);
        ~RequestTasks();

        // non-concurrent (from the request)
        SmartPtr<ModuleTask> getNextTask();
        bool hasWork();
        bool hasPendingEvent();
        bool hasError();
        void addTask(API::EVENT::Type event,
                     Request& request,
                     ModuleWrapper& module,
                     API::IBuffer* buffer);
        void needRead();
        void needWrite(API::IBuffer* buffer);
        void reset();

        // concurrent (from the request manager)
        void notifyHasError();
        void handleSocketEvent(SOCKET_EVENT::Type evt);
        void addEndTask(Request& request, ModuleWrapper& module);
        void addCallLaterTask(Request& request, ModuleWrapper& module, API::uint32_t delay);
    };
}

#endif /* !__REQUESTTASKS_HPP__ */

