
#ifndef __CONFIGURATIONPARSEREXCEPTION_HPP__
# define __CONFIGURATIONPARSEREXCEPTION_HPP__

# include <stdexcept>

namespace zhttpd
{
    namespace parser
    {
        class ConfigurationParserException : public std::runtime_error
        {
        public:
            ConfigurationParserException(std::string const& what) : std::runtime_error(what) {}
        };
    }
}

#endif

