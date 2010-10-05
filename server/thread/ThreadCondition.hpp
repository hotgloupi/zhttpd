
#ifndef __THREADCONDITION_HPP__
# define __THREADCONDITION_HPP__

# ifdef _WIN32
#  include "ThreadConditionWindows.hpp"
# else
#  include "ThreadConditionUnix.hpp"
# endif // !_WIN32

namespace ZHTTPD
{
    typedef IMPLEMENTATION::ThreadCondition     ThreadCondition;
}

#endif //!__THREADCONDITION_HPP__
