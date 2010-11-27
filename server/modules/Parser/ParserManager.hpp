
#ifndef __PARSERMANAGER_HPP__
# define __PARSERMANAGER_HPP__

# include <map>

# include "modules/common/AbstractManager.hpp"
# include "Parser.hpp"

#ifdef WIN32
# define __TMP_DELETE DELETE
# undef DELETE
#endif

namespace zhttpd
{
    namespace mod
    {
        class ParserManager : public StatefullManager<Parser>
        {
            private:
                std::map<std::string, api::http_method::Type> _methods;

            public:
                ParserManager() :
                    StatefullManager<Parser>("mod_parser", zhttpd::api::category::POSTINPUT)
                {
                    this->_methods["GET"] = api::http_method::GET;
                    this->_methods["POST"] = api::http_method::POST;
                    this->_methods["PUT"] = api::http_method::PUT;
                    this->_methods["DELETE"] = api::http_method::DELETE;
                    this->_methods["TRACE"] = api::http_method::TRACE;
                    this->_methods["OPTIONS"] = api::http_method::OPTIONS;
                    this->_methods["CONNECT"] = api::http_method::CONNECT;
                    this->_methods["HEAD"] = api::http_method::HEAD;
                }

                api::http_method::Type getMethod(std::string const& str) const
                {
                    std::map<std::string, api::http_method::Type>::const_iterator it = this->_methods.find(str);
                    if (it == this->_methods.end())
                        return api::http_method::UNDEFINED;
                    return it->second;
                }
        };
    }
}

#ifdef WIN32
# define DELETE __TMP_DELETE
# undef __TMP_DELETE
#endif

#endif
