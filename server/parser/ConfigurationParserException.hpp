
#ifndef __CONFIGURATIONPARSEREXCEPTION_HPP__
# define __CONFIGURATIONPARSEREXCEPTION_HPP__

# include <stdexcept>

namespace ZHTTPD
{
    namespace PARSER
    {
        class ConfigurationParserException : public std::runtime_error
        {
        public:
            ConfigurationParserException(std::string const& what) : std::runtime_error(what) {}
        };
    }
}

#endif

