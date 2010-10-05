
#ifndef __PARSERMANAGER_HPP__
# define __PARSERMANAGER_HPP__

# include <map>

# include "modules/common/AbstractManager.hpp"
# include "Parser.hpp"

#ifdef WIN32
# define __TMP_DELETE DELETE
# undef DELETE
#endif

namespace ZHTTPD
{
    namespace MOD
    {
        class ParserManager : public StatefullManager<Parser>
        {
            private:
                std::map<std::string, API::HTTP_METHOD::Type> _methods;

            public:
                ParserManager() :
                    StatefullManager<Parser>("mod_parser", ZHTTPD::API::CATEGORY::POSTINPUT)
                {
                    this->_methods["GET"] = API::HTTP_METHOD::GET;
                    this->_methods["POST"] = API::HTTP_METHOD::POST;
                    this->_methods["PUT"] = API::HTTP_METHOD::PUT;
                    this->_methods["DELETE"] = API::HTTP_METHOD::DELETE;
                    this->_methods["TRACE"] = API::HTTP_METHOD::TRACE;
                    this->_methods["OPTIONS"] = API::HTTP_METHOD::OPTIONS;
                    this->_methods["CONNECT"] = API::HTTP_METHOD::CONNECT;
                    this->_methods["HEAD"] = API::HTTP_METHOD::HEAD;
                }

                API::HTTP_METHOD::Type getMethod(std::string const& str) const
                {
                    std::map<std::string, API::HTTP_METHOD::Type>::const_iterator it = this->_methods.find(str);
                    if (it == this->_methods.end())
                        return API::HTTP_METHOD::UNDEFINED;
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
