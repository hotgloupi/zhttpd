
#ifndef __ATOMIC_HPP__
# define __ATOMIC_HPP__

#ifdef _WIN32
# include "AtomicWindows.hpp"
#else
# include "AtomicUnix.hpp"
#endif // !_WIN32

namespace zhttpd
{
    typedef implementation::Atomic      Atomic;
}

#endif // !__ATOMIC_HPP__
