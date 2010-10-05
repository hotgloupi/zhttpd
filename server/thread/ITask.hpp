
#ifndef __ITASK_HPP__
# define __ITASK_HPP__

namespace ZHTTPD
{
    class ITask
    {
    public:
        virtual ~ITask() {}
        virtual void run() = 0;
    };
}

#endif /* !__ITASK_HPP__ */

