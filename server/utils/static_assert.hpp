
#ifndef __STATIC_ASSERT_HPP__
# define __STATIC_ASSERT_HPP__

template<bool> class _static_assert
{
};

template<> class _static_assert<false>
{
private:
    _static_assert();
};

# define static_assert(test) _static_assert<(test) != 0>()

#endif // !__STATIC_ASSERT_HPP__
