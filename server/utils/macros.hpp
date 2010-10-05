
#ifndef __MACROS_HPP__
# define __MACROS_HPP__

# define ZHTTPD_FULLNAME "zhttp/0.1"

# define ZHTTPD_DELETE(ptr) \
    do {\
        if (ptr != 0) {\
            delete ptr; \
            ptr = 0; \
        }\
    } while(0)


# ifdef ZHTTPD_DEBUG
#  include <map>
#  include <string>
#  include <typeinfo>
#  include "utils/Timer.hpp"
#  include "thread/Mutex.hpp"
#  include "utils/Logger.hpp"
extern std::map<std::string, ZHTTPD::API::uint64_t>* __ZHTTPD_LOCK_times;
extern ZHTTPD::Mutex*                                __ZHTTPD_LOCK_times_mutex;
extern std::map<std::string, ZHTTPD::API::uint64_t>* __ZHTTPD_MOD_times;
extern std::map<std::string, ZHTTPD::API::uint32_t>* __ZHTTPD_MOD_counts;
extern ZHTTPD::Timer*                                __ZHTTPD_timer;
extern ZHTTPD::Mutex*                                __ZHTTPD_MOD_mutex;

void ZHTTPD_DEBUG_init();
void ZHTTPD_DEBUG_print_maps();
void ZHTTPD_DEBUG_release();

#  define ZHTTPD_LOCK(mutex)                                                                       \
    do {                                                                                        \
        if ((mutex).tryLock() == false)                                                         \
        {                                                                                       \
            ZHTTPD::API::uint64_t start = __ZHTTPD_timer->getPreciseElapsedTime();                    \
            (mutex).lock();                                                                     \
            ZHTTPD::API::uint64_t delta = __ZHTTPD_timer->getPreciseElapsedTime() - start;            \
            __ZHTTPD_LOCK_times_mutex->lock();                                                     \
            (*__ZHTTPD_LOCK_times)["L " + ZHTTPD_LOGGER_FUNC_NAME] += delta;                          \
            __ZHTTPD_LOCK_times_mutex->unlock();                                                   \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            __ZHTTPD_LOCK_times_mutex->lock();                                                     \
            (*__ZHTTPD_LOCK_times)["L " + ZHTTPD_LOGGER_FUNC_NAME] += 0;                              \
            __ZHTTPD_LOCK_times_mutex->unlock();                                                   \
        }                                                                                       \
    } while(0)

#  define ZHTTPD_RLOCK(mutex)                                                                      \
    do {                                                                                        \
        if ((mutex).tryLockRead() == false)                                                     \
        {                                                                                       \
            ZHTTPD::API::uint64_t start = __ZHTTPD_timer->getPreciseElapsedTime();                    \
            (mutex).lockRead();                                                                 \
            ZHTTPD::API::uint64_t delta = __ZHTTPD_timer->getPreciseElapsedTime() - start;            \
            __ZHTTPD_LOCK_times_mutex->lock();                                                     \
            (*__ZHTTPD_LOCK_times)["R " + ZHTTPD_LOGGER_FUNC_NAME] += delta;                          \
            __ZHTTPD_LOCK_times_mutex->unlock();                                                   \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            __ZHTTPD_LOCK_times_mutex->lock();                                                     \
            (*__ZHTTPD_LOCK_times)["R " + ZHTTPD_LOGGER_FUNC_NAME] += 0;                              \
            __ZHTTPD_LOCK_times_mutex->unlock();                                                   \
        }                                                                                       \
    } while(0)

// XXX use MOD_times_mutex in this macro
#  define ZHTTPD_WLOCK(mutex)                                                                      \
    do {                                                                                        \
        if ((mutex).tryLockWrite() == false)                                                    \
        {                                                                                       \
            ZHTTPD::API::uint64_t start = __ZHTTPD_timer->getPreciseElapsedTime();                    \
            (mutex).lockWrite();                                                                \
            ZHTTPD::API::uint64_t delta = __ZHTTPD_timer->getPreciseElapsedTime() - start;            \
            __ZHTTPD_LOCK_times_mutex->lock();                                                     \
            (*__ZHTTPD_LOCK_times)["W " + ZHTTPD_LOGGER_FUNC_NAME] += delta;                          \
            __ZHTTPD_LOCK_times_mutex->unlock();                                                   \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            __ZHTTPD_LOCK_times_mutex->lock();                                                     \
            (*__ZHTTPD_LOCK_times)["W " + ZHTTPD_LOGGER_FUNC_NAME] += 0;                              \
            __ZHTTPD_LOCK_times_mutex->unlock();                                                   \
        }                                                                                       \
    } while(0)

# define ZHTTPD_ASSERT_LOCK(mutex)         assert(mutex.tryLock())
# define ZHTTPD_ASSERT_UNLOCK(mutex)       mutex.unlock()

#  define ZHTTPD_MOD_START                                                                         \
    __ZHTTPD_MOD_mutex->lock();                                                                    \
    ZHTTPD::API::uint64_t __ZHTTPD_MOD_start = __ZHTTPD_timer->getPreciseElapsedTime();                  \
    __ZHTTPD_MOD_mutex->unlock();                                                                  \


#  define ZHTTPD_MOD_END(name)                                                                     \
    do {                                                                                        \
        __ZHTTPD_MOD_mutex->lock();                                                                \
        ZHTTPD::API::uint64_t delta = __ZHTTPD_timer->getPreciseElapsedTime() - __ZHTTPD_MOD_start;      \
        (*__ZHTTPD_MOD_times)[name] += delta;                                                      \
        (*__ZHTTPD_MOD_counts)[name] += 1;                                                         \
        __ZHTTPD_MOD_mutex->unlock();                                                              \
    } while(0)

# else /* !ZHTTPD_DEBUG */
#  define ZHTTPD_ASSERT_LOCK(mutex)        (static_cast<void>(0))
#  define ZHTTPD_ASSERT_UNLOCK(mutex)      (static_cast<void>(0))
#  define ZHTTPD_LOCK(mutex)               mutex.lock()
#  define ZHTTPD_RLOCK(mutex)              mutex.lockRead();
#  define ZHTTPD_WLOCK(mutex)              mutex.lockWrite();
#  define ZHTTPD_MOD_START
#  define ZHTTPD_MOD_END(name)             (static_cast<void>(0))
# endif

# define ZHTTPD_UNLOCK(mutex)              mutex.unlock()
# define ZHTTPD_RUNLOCK(mutex)             mutex.unlockRead();
# define ZHTTPD_WUNLOCK(mutex)             mutex.unlockWrite();

#endif /* !__MACROS_HPP__ */

