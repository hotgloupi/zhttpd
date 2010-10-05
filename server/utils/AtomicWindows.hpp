
#ifdef _WIN32
# ifndef __ATOMICWINDOWS_HPP__
#  define __ATOMICWINDOWS_HPP__

#  include <Windows.h>
#  include <cassert>

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        struct Atomic
        {
            template<typename T> static inline T increment(volatile T *counter)
            {
                assert(sizeof(T) == sizeof(LONG));
                return static_cast<T>(::InterlockedIncrement(reinterpret_cast<volatile LONG*>(counter)));
            }

            template<typename T> static inline T decrement(volatile T *counter)
            {
                assert(sizeof(T) == sizeof(LONG));
                return static_cast<T>(::InterlockedDecrement(reinterpret_cast<volatile LONG*>(counter)));
            }

            template<typename T1, typename T2> static inline T1 add(volatile T1 *ptr, T2 value)
            {
                assert(sizeof(T1) == sizeof(LONG));
                return static_cast<T1>(::InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(value)));
            }

            template<typename T> static inline T readAndClear(volatile T *ptr)
            {
                assert(sizeof(T) == sizeof(LONG));
                return static_cast<T>(::InterlockedExchange(reinterpret_cast<volatile LONG*>(ptr), 0));
            }
        };
    }
}

# endif // !__ATOMICWINDOWS_HPP__
#endif // _WIN32
