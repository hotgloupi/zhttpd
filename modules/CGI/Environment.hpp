
#ifndef __ENVIRONMENT_HPP__
# define __ENVIRONMENT_HPP__

# include <string>
# include <map>

# include "api/IRequest.hpp"

class Environment
{
    typedef std::map<std::string, std::string> env_t;

private:
    char**  _c_env;
    bool    _is_set;
    env_t   _environment;

public:
    Environment();
    ~Environment();
    void copyHeadersToEnvironment(ZHTTPD::API::IRequest* request);
    void setEnvironmentVariable(char const* _key, char const* _value);
    char* const* getEnvironmentArray() const;
    env_t const& getEnvironmentList() const;

private:
    void _fillEnvironment();
};

#endif  /* ENVIRONMENT_HPP */
