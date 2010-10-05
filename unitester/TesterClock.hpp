
#ifndef CLOCK_HPP_
# define CLOCK_HPP_

#ifdef _WIN32 //windows
#include <windows.h>
#else //unix
#include <sys/time.h>
#endif

namespace Test
{

    class TesterClock
    {
    public:
        TesterClock()
        {
            this->reset();
        }
#ifdef _WIN32 //windows

        void reset()
        {
            LARGE_INTEGER freq;
            ::QueryPerformanceFrequency(&freq);
            this->_frequence = 1.0f / freq.QuadPart;
            ::QueryPerformanceCounter(&this->_base_counter);
        }

        float getElapsedTime() const
        {
            LARGE_INTEGER curCount;
            ::QueryPerformanceCounter(&curCount);
            return (curCount.QuadPart - this->_base_counter.QuadPart) * this->_frequence;
        }

#else //unix

        void reset()
        {
            gettimeofday(&this->_base_time, 0);
        }

        float getElapsedTime() const
        {
            float t;

            struct timeval tv;
            gettimeofday(&tv, 0);

            t = (tv.tv_sec - this->_base_time.tv_sec) +
                (tv.tv_usec - this->_base_time.tv_usec) * 0.000001f;
            return t;
        }

#endif //OS

    private:
#ifdef _WIN32 //windows
        LARGE_INTEGER _base_counter;
        float _frequence;
#else //unix
        struct timeval _base_time;
#endif
    };

}

#endif // !CLOCK_HPP_
