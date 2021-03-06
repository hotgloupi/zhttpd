
#include <string>
#include <cstring>
#include <map>
#include <list>
#include <algorithm>

#include "api/IRequest.hpp"
#include "api/IBuffer.hpp"
#include "api/types.hpp"
#include "utils/macros.hpp"

#include "Environment.hpp"

// XXX improve all this shit

Environment::Environment() : _c_env(0)
{
}

Environment::~Environment()
{
    if (this->_c_env != 0)
    {
        zhttpd::api::size_t i = 0;
        while (this->_c_env[i] != 0)
        {
            delete [] this->_c_env[i];
            this->_c_env[i] = 0;
            ++i;
        }
        delete [] this->_c_env;
        this->_c_env = 0;
    }
}

void Environment::copyHeadersToEnvironment(zhttpd::api::IRequest* request)
{
    typedef std::list<std::string const*> header_keys_t;
    header_keys_t header_keys = request->getRequestHeaderKeys();
    header_keys_t::const_iterator it = header_keys.begin();
    header_keys_t::const_iterator end = header_keys.end();

    for (; it != end; ++it)
    {
        std::string const& value = request->getRequestHeader(*(*it));
        std::string key = *(*it);
        std::replace(key.begin(), key.end(), '-', '_');
        std::transform(key.begin(), key.end(), key.begin(), toupper);
        if (key != "CONTENT_LENGTH" && key != "CONTENT_TYPE")
        {
            key = "HTTP_" + key;
        }
        this->_environment[key] = value;
        this->setEnvironmentVariable(key.c_str(), value.c_str());
    }
    this->_fillEnvironment();
}

char* const* Environment::getEnvironmentArray() const
{
    return this->_c_env;
}

Environment::env_t const& Environment::getEnvironmentList() const
{
    return this->_environment;
}

void Environment::setEnvironmentVariable(char const* key, char const* value)
{
    this->_environment[key] = value;
}

void   Environment::_fillEnvironment()
{
    std::map<std::string, std::string>::const_iterator          it;
    std::map<std::string, std::string>::const_iterator          ite;
    std::string                                                 temp_str;
    zhttpd::api::uint32_t                                          i;

    i = 0;
    it = this->_environment.begin();
    ite = this->_environment.end();
    this->_c_env = new char*[this->_environment.size() + 1];
    for (; it != ite; it++)
    {
        temp_str = it->first;
        temp_str.append("=");
        temp_str.append(it->second);
        this->_c_env[i] = new char[temp_str.size() + 1];
        ::strncpy(this->_c_env[i], temp_str.c_str(), temp_str.size());
        this->_c_env[i][temp_str.size()] = 0;
        i++;
    }
    this->_c_env[i] = 0;
}
