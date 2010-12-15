
#ifndef __ITHREADCONDITION_HPP__
# define __ITHREADCONDITION_HPP__

namespace zhttpd
{
    class IThreadCondition
    {
    public:
        virtual ~IThreadCondition() {}
        virtual void wait() = 0;
        virtual void wake() = 0;
        virtual void wakeAll() = 0;
    };
}

#endif /* !__ITHREADCONDITION_HPP__ */

