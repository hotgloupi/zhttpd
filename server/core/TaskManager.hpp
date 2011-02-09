
#ifndef __TASKMANAGER_HPP__
# define __TASKMANAGER_HPP__

# include <list>
# include <set>

# include "core/Request.hpp"

namespace zhttpd
{
    template<template <class> class Allocator> class RequestManager;

    template<template <class> class Allocator>
    class TaskManager
    {
    public:
        typedef std::list<Request*> request_list_t;
        typedef std::set<Request*> request_set_t;

    private:
        request_set_t               _requests;
        request_set_t               _pending_add_requests;
        request_set_t               _pending_del_requests;
        RequestManager<Allocator>&  _request_manager;

    public:
        TaskManager(RequestManager<Allocator>& request_manager);
        virtual ~TaskManager();
        void notifyEndTask(Request& request);
        void startRequest(Request& request);
        void stopRequest(Request& request);
        virtual void run();

    private:
        api::size_t _giveWork();
        api::size_t _addPendingRequests();
        api::size_t _delPendingRequests();
    };
}

#endif /* !__TASKMANAGER_HPP__ */

