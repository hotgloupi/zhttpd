
#include <iostream>
#include "Regex.hpp"

using namespace ZHTTPD;

#define SKIP_STARS(s)                               \
do {                                                \
                for(; *(s) == '*' ; ++(s)){}        \
} while (0)

bool Regex::match(char const* pattern, char const* str)
{
    while (*pattern && *str)
    {
        if (*pattern == '*')
        {
            SKIP_STARS(pattern);
            while (*str)
            {
                if (*pattern == '?')
                    return match(pattern, str);
                else if (*str != *pattern)
                    ++str;
                else if (match(str, pattern))
                    return true;
                else if (*str)
                    ++str;
            }
        }
        else if (*pattern == '?')
        {
            ++str;
            ++pattern;
        }
        else if (*pattern != *str)
            return false;
        else
        {
            ++str;
            ++pattern;
        }
    }
    SKIP_STARS(pattern);

    return (*str == '\0' && *pattern == '\0');
}

#undef SKIP_STARS

bool Regex::match(std::string const& pattern, std::string const& str)
{
    return match(pattern.c_str(), str.c_str());
}

