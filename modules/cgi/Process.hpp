#ifndef PROCESS_HPP
#define PROCESS_HPP

namespace PROCESS_STATUS
{
    enum Type
    {
        STARTED = 1,
        RUN = 2,
        FINISHED = 3,
    };
}

# ifdef     WIN32
#  include "WindowsProcess.hpp"
typedef WindowsProcess ProcessHandler;
# else
#  include "UnixProcess.hpp"
typedef UnixProcess ProcessHandler;
# endif     // WIN32


#endif // PROCESS_HPP
