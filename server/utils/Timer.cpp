
#ifdef _WIN32 //windows
# include <windows.h>
#else //unix
# include <sys/time.h>
# include <unistd.h>
#endif

#include "Timer.hpp"

using namespace ZHTTPD;

Timer::Timer()
{
    this->reset();
}

void Timer::reset()
{
#ifdef _WIN32 //windows
    ::QueryPerformanceFrequency(&this->_frequence);
    LARGE_INTEGER curCount;
    ::QueryPerformanceCounter(&curCount);
    this->_base_time = curCount.QuadPart;
#else //unix
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    this->_base_time = tv.tv_sec;
    this->_base_time *= 1000;
    this->_base_time += tv.tv_usec / 1000;
#endif
}

API::uint32_t Timer::getElapsedTime() const
{
#ifdef _WIN32 //windows
    LARGE_INTEGER curCount;
    ::QueryPerformanceCounter(&curCount);
    return (curCount.QuadPart - this->_base_time) * 1000 / this->_frequence.QuadPart;
#else //unix
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    uint64_t nu_time;
    nu_time = tv.tv_sec;
    nu_time *= 1000;
    nu_time += tv.tv_usec / 1000;
    return nu_time - this->_base_time;
#endif
}

API::uint64_t Timer::getPreciseElapsedTime() const
{
#ifdef _WIN32 //windows
    LARGE_INTEGER curCount;
    ::QueryPerformanceCounter(&curCount);
    return (curCount.QuadPart - this->_base_time) * 1000 / this->_frequence.QuadPart;
    //XXX adapter pour avoir les micro secondes
#else //unix
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    uint64_t nu_time;
    nu_time = tv.tv_sec;
    nu_time *= 1000000;
    nu_time += tv.tv_usec;
    return nu_time - this->_base_time * 1000;
#endif
}

void Timer::sleep(API::uint32_t ms)
{
#ifdef _WIN32
    ::Sleep(ms);
#else // Unix
    ::usleep(ms * 1000L);
#endif
}
