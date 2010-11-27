
#ifndef __CRYPTOLOCK_HPP__
# define __CRYPTOLOCK_HPP__

# include "thread/Mutex.hpp"

struct CRYPTO_dynlock_value
{
    zhttpd::Mutex mutex;
};

namespace zhttpd
{

    struct CryptoLock
    {
        static struct CRYPTO_dynlock_value* createMutex(char const* file, int line);
        static void lockMutex(int mode, struct CRYPTO_dynlock_value* l, char const* file, int line);
        static void destroyMutex(struct CRYPTO_dynlock_value* l, char const* file, int line);

        static void staticLock(int mode, int n, char const* file, int line);

#ifndef _WIN32
        static unsigned long getId();
#endif
    };

}

#endif // !__CRYPTOLOCK_HPP__
