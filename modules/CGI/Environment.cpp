
#include <string>
#include <cstring>
#include <map>
#include <list>
#include <algorithm>

#include "api/IRequest.hpp"
#include "api/IBuffer.hpp"
#include "api/types.hpp"

#include "Environment.hpp"

Environment::Environment()
{
    _is_set = false;
}

Environment::~Environment()
{
    /*delete *this->_c_env;*/
    /*delete this->_c_env;*/
}

void Environment::copyHeadersToEnvironment(ZHTTPD::API::IRequest* request)
{
    typedef std::list<std::string const*> header_keys_t;
    header_keys_t header_keys = request->getRequestHeaderKeys();
    header_keys_t::const_iterator it = header_keys.begin();
    header_keys_t::const_iterator end = header_keys.end();

    for (; it != end; ++it)
    {
        std::string const& value = request->getRequestHeader(*(*it));
        this->setEnvironmentVariable((*it)->c_str(), value.c_str());
    }
    this->_fillEnvironment();
}

char* const* Environment::getEnvironmentArray() const
{
    if (this->_is_set)
    {
        return this->_c_env;
    }
    else
    {
        return 0;
    }
}

Environment::env_t const& Environment::getEnvironmentList() const
{
    return this->_environment;
}

void Environment::setEnvironmentVariable(char const* _key, char const* _value)
{
    std::string*    key = new std::string(_key);
    std::string*    value = new std::string(_value);
    char            replace[2];

    replace[0] = '-';
    replace[1] = '_';
    std::replace(key->begin(), key->end(), replace[0], replace[1]);
    std::transform(key->begin(), key->end(), key->begin(), toupper);
    *key = (*key == "CONTENT_LENGHT" || *key == "CONTENT_TYPE") ? "HTTP_" +  *key : *key;
    this->_environment[*key] = *value;
    delete key;
    delete value;
}

void   Environment::_fillEnvironment()
{
    std::map<std::string, std::string>::const_iterator          it;
    std::map<std::string, std::string>::const_iterator          ite;
    std::string                                                 temp_str;
    ZHTTPD::API::uint32_t                                          i;

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
    this->_is_set = true;
}
