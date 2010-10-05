
#ifndef _WIN32
#include <pthread.h>
#endif

#include <openssl/crypto.h>
#include <iostream>

#include "CryptoLock.hpp"

using namespace ZHTTPD;

struct CRYPTO_dynlock_value* CryptoLock::createMutex(char const*, int)
{
    return new struct CRYPTO_dynlock_value;
}

void CryptoLock::lockMutex(int mode, struct CRYPTO_dynlock_value* l, char const*, int)
{
    if (mode & CRYPTO_LOCK)
        l->mutex.lock();
    else
        l->mutex.unlock();

}

void CryptoLock::destroyMutex(struct CRYPTO_dynlock_value* l, char const*, int)
{
    delete l;
}


void CryptoLock::staticLock(int mode, int n, char const*, int)
{
    static Mutex* staticMutex = 0;

    if (staticMutex == 0)
        staticMutex = new Mutex[CRYPTO_num_locks()];

    if (mode & CRYPTO_LOCK)
        staticMutex[n].lock();
    else
        staticMutex[n].unlock();
}

#ifndef _WIN32

unsigned long CryptoLock::getId()
{
    return (unsigned long)pthread_self();
}

#endif
