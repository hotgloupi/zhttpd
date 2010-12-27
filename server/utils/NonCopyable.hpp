
#ifndef __NONCOPYABLE_HPP__
# define __NONCOPYABLE_HPP__

namespace zhttpd
{
    class NonCopyable
    {
    private:
        NonCopyable(NonCopyable const& ref);
        NonCopyable& operator =(NonCopyable const& ref);
    protected:
        NonCopyable() {}
        virtual ~NonCopyable() {}
    };
}

#endif // !__NONCOPYABLE_HPP__
