
#ifndef __CGI_HPP__
# define __CGI_HPP__

# include <sys/types.h>
# include <list>
# include <utility>
# include <map>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"
# include "api/types.hpp"

# include "Process.hpp"
# include "Environment.hpp"
# include "modules/common/AbstractManager.hpp"

class CGI : public ZHTTPD::API::IModule
{
public:
    static ZHTTPD::API::size_t const           BUFFER_SIZE;
    typedef ZHTTPD::API::IBuffer               buffer_t;
    typedef ZHTTPD::API::IRequest              request_t;
    typedef ZHTTPD::API::HTTP_METHOD::Type     method_t;
    typedef bool (CGI::*action_t)(request_t* request, buffer_t* buffer);

private:
    static action_t const                   _actions[ZHTTPD::API::EVENT::ON_END + 1];
    ZHTTPD::MOD::POLICIES::MapConfigurationPolicy& _module_manager;
    ZHTTPD::API::HTTP_METHOD::Type             _method;
    ProcessHandler                          _process;
    std::list<std::string>                  _process_arguments;
    Environment                             _environment;
    ZHTTPD::API::size_t                        _request_content_length;
    std::map<std::string, std::string>      _executable_handler;
    std::string                             _script_name;
    std::string                             _extension;
    bool                                    _started;
    bool                                    _end;
    bool                                    _headers;

public:
    CGI(ZHTTPD::MOD::POLICIES::MapConfigurationPolicy* manager);
    ~CGI();
    bool processRequest(ZHTTPD::API::EVENT::Type event, request_t* request, buffer_t* buffer);
    bool isStarted() const;

private:
    bool        _processOnRequestData(request_t* request, buffer_t* buffer);
    bool        _processOnIdle(request_t* request, buffer_t* buffer);
    bool        _processOnEnd(request_t* request, buffer_t* buffer);
    void        _initialize(request_t* request);
    void        _fillEnvironment(request_t* request);
    void        _debug(request_t* request, buffer_t* buffer);
    bool        _checkContentLength(ZHTTPD::API::uint32_t request_content_length);
    bool        _checkExtension(request_t* request);
    void        _parseHeaders(request_t* request, buffer_t* buffer);
    void        _parseHeadersLine(request_t* request, std::string const& line);
    buffer_t*   _extractHeaders(request_t* request, buffer_t* buffer);


};

#endif /* !__CGI_HPP__ */
