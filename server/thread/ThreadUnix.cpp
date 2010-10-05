#ifndef _WIN32
#include <pthread.h>
#include <stdexcept>

#include "thread/ITask.hpp"
#include "utils/NonCopyable.hpp"

#include "thread/ThreadUnix.hpp"

using namespace ZHTTPD;
using namespace ZHTTPD::IMPLEMENTATION;


void* Thread::_run(Thread* thread)
{
    thread->_callback_instance->run();
    return 0;
}

Thread::Thread(ITask* instance) : _running(true), _callback_instance(instance)
{
    this->_thread = new pthread_t;
    if (::pthread_create(this->_thread, 0, reinterpret_cast<pthread_run_t>(Thread::_run), this) != 0)
        throw std::runtime_error("pthread_create error.");
}

void Thread::join()
{
    if (!this->_running)
        return;
    ::pthread_join(*this->_thread, 0);
    this->_running = false;
}

void Thread::quit()
{
    if (this->_thread)
    {
        ::pthread_exit(this->_thread);
        delete this->_thread;
        this->_thread = 0;
    }
    this->_running = false;
}

Thread::~Thread()
{
    if (this->_running)
        pthread_detach(*this->_thread);
    this->_running = false;
    if (this->_thread)
    {
        delete this->_thread;
        this->_thread = 0;
    }
}
#endif // !_WIN32
