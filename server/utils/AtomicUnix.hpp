
#ifndef _WIN32
# ifndef __ATOMICUNIX_HPP__
#  define __ATOMICUNIX_HPP__

#  include <sys/types.h>

#  include "utils/static_assert.hpp"

#  ifdef __FreeBSD__
#   include <machine/atomic.h>
#   define ZHTTPD_ATOMIC_ADDFETCH(ptr, i) (::atomic_fetchadd_int(reinterpret_cast<volatile unsigned int*>(ptr), i) + i)
#   define ZHTTPD_ATOMIC_READANDCLEAR(ptr) (::atomic_readandclear_int(reinterpret_cast<volatile unsigned int*>(ptr)))
#  else
#   define ZHTTPD_ATOMIC_ADDFETCH(ptr, i) ::__sync_add_and_fetch(reinterpret_cast<volatile unsigned int*>(ptr), i)
#   define ZHTTPD_ATOMIC_READANDCLEAR(ptr) ::__sync_fetch_and_and(reinterpret_cast<volatile unsigned int*>(ptr), 0)
#  endif

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        struct Atomic
        {
            template<typename T> static inline T increment(volatile T *counter)
            {
                static_assert(sizeof(T) == sizeof(unsigned int));
                return static_cast<T>(ZHTTPD_ATOMIC_ADDFETCH(counter, 1));
            }

            template<typename T> static inline T decrement(volatile T *counter)
            {
                static_assert(sizeof(T) == sizeof(unsigned int));
                return static_cast<T>(ZHTTPD_ATOMIC_ADDFETCH(counter, -1));
            }

            template<typename T> static inline T add(volatile T* counter, T value)
            {
                static_assert(sizeof(T) == sizeof(unsigned int));
                return static_cast<T>(ZHTTPD_ATOMIC_ADDFETCH(counter, value));
            }

//            template<typename T> static inline T sub(volatile T* counter, T value)
//            {
//                static_assert(sizeof(T) == sizeof(unsigned int));
//                return static_cast<T>(ZHTTPD_ATOMIC_SUBFETCH(counter, value));
//            }

            template<typename T> static inline T readAndClear(volatile T *current)
            {
                static_assert(sizeof(T) == sizeof(unsigned int));
                return static_cast<T>(ZHTTPD_ATOMIC_READANDCLEAR(current));
            }
        };
    }
}

# undef ZHTTPD_ATOMIC_FETCHADD

# endif // !__ATOMICUNIX_HPP__
#endif // !_WIN32
