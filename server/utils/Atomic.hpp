
#ifndef __ATOMIC_HPP__
# define __ATOMIC_HPP__

#ifdef _WIN32
# include "AtomicWindows.hpp"
#else
# include "AtomicUnix.hpp"
#endif // !_WIN32

namespace ZHTTPD
{
    typedef IMPLEMENTATION::Atomic      Atomic;
}

#endif // !__ATOMIC_HPP__
