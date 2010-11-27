
#ifndef __ICONDITION_HPP__
# define __ICONDITION_HPP__

namespace zhttpd
{
    class ICondition
    {
    public:
        virtual ~ICondition() {}
        virtual bool checkCondition() = 0;
    };
}

#endif /* !__ICONDITION_HPP__ */

