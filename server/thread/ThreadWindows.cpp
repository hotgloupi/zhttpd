
#ifdef _WIN32

#include <Windows.h>
#include <stdexcept>

#include "thread/ITask.hpp"
#include "utils/NonCopyable.hpp"

#include "thread/ThreadWindows.hpp"

using namespace ZHTTPD;
using namespace ZHTTPD::IMPLEMENTATION;

DWORD Thread::_run(Thread* thread)
{
    thread->_callback_instance->run();
    return 0;
}

Thread::Thread(ITask* instance) : _callback_instance(instance), _running(true)
{
    this->_thread = ::CreateThread(NULL,
                                    0,
                                    reinterpret_cast<LPTHREAD_START_ROUTINE>(Thread::_run),
                                    this,
                                    0,
                                    0);
    if (this->_thread == 0)
        throw std::runtime_error(std::string("CreateThread fail with code: ") + (int)::GetLastError());
}

void Thread::join()
{
    if (!this->_running)
        return;
    ::WaitForSingleObject(this->_thread, INFINITE);
    this->_running = false;
}

void Thread::quit()
{
    if (this->_thread)
    {
        ::TerminateThread(this->_thread, 0);
        this->_thread = 0;
    }
    this->_running = false;
}

Thread::~Thread()
{
    this->quit();
}

#endif // _WIN32
