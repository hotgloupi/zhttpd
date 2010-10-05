
#ifndef __TIMER_HPP__
# define __TIMER_HPP__

#ifdef _WIN32
# include <Windows.h>
#endif // _WIN32

# include "api/types.hpp"

namespace ZHTTPD
{
    class Timer
    {
    private:
        API::uint64_t _base_time;
#ifdef _WIN32
        LARGE_INTEGER _frequence;
#endif // _WIN32

    public:
        Timer();
        void reset();
        /**
         * Retourne le temps écoulé en millisecondes
         */
        API::uint32_t getElapsedTime() const;
        API::uint64_t getPreciseElapsedTime() const;
        static void sleep(API::uint32_t ms);
    };
}

#endif // !__TIMER_HPP__
