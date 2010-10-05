#ifndef __RWLOCK_HPP__
# define __RWLOCK_HPP__

#ifdef _WIN32
# include "RWLockWindows.hpp"
#else
# include "RWLockUnix.hpp"
#endif // !_WIN32

#endif // !__RWLOCK_HPP__
