
#ifndef __NONCOPYABLE_HPP__
# define __NONCOPYABLE_HPP__

namespace ZHTTPD
{
    class NonCopyable
    {
    private:
        NonCopyable(NonCopyable const& ref);
        NonCopyable& operator =(NonCopyable const& ref);
    protected:
        NonCopyable() {}
        ~NonCopyable() {}
    };
}

#endif // !__NONCOPYABLE_HPP__
