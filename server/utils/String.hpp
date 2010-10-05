#ifndef __STRING_HPP__
# define __STRING_HPP__

# include <string>
# include <sstream>

inline std::string operator +(std::string const& str, int nbr)
{
    std::stringstream ss;
    ss << str << nbr;
    return ss.str();
}

inline std::string operator +(int nbr, std::string const& str)
{
    std::stringstream ss;
    ss << nbr << str;
    return ss.str();
}

inline std::string operator +(std::string const& str, void* nbr)
{
    std::stringstream ss;
    ss << str << nbr;
    return ss.str();
}

inline std::string operator +(void* nbr, std::string const& str)
{
    std::stringstream ss;
    ss << nbr << str;
    return ss.str();
}

#endif // !__STRING_HPP__
