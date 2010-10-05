#ifndef _UTILS_HPP
# define _UTILS_HPP

# include <utility>
# include <string>
# include <sstream>

template<typename RET, typename T, typename Trait, typename Alloc>
RET     ss_atoi(const std::basic_string<T, Trait, Alloc>& string)
{
    std::basic_istringstream<T, Trait, Alloc>   temp_ss(string);
    RET number;
    temp_ss >> number;

    return (number);
}

template <typename T>
std::basic_string<T> ss_itoa(long number, unsigned width = 0)
{
    std::basic_stringstream<T>                  temp_ss;
    if (width)
    {
        temp_ss.fill('0');
        temp_ss.width(width);
    }
    temp_ss << number;

    return (temp_ss.str());
}

#endif  /* _UTILS_HPP */
