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

inline std::list<std::string> operator /(std::string const& s, char delim)
{
    std::list<std::string> elements;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elements.push_back(item);
    return elements;
}

#endif // !__STRING_HPP__
