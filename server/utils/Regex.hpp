
#ifndef REGEX_HPP__
# define REGEX_HPP__

#include <string>

namespace ZHTTPD
{
    class Regex
    {
    public:
        static bool match(char const* pattern, char const* str);
        static bool match(std::string const& pattern, std::string const& str);
    };
}

#endif // REGEX_HPP__

