
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

class CGI : public zhttpd::api::IModule
{
public:
    static zhttpd::api::size_t const           BUFFER_SIZE;
    typedef zhttpd::api::IBuffer               buffer_t;
    typedef zhttpd::api::IRequest              request_t;
    typedef zhttpd::api::http_method::Type     method_t;
    typedef bool (CGI::*action_t)(request_t* request, buffer_t* buffer);

private:
    static action_t const                   _actions[zhttpd::api::event::ON_END + 1];
    zhttpd::mod::POLICIES::MapConfigurationPolicy& _module_manager;
    zhttpd::api::http_method::Type             _method;
    ProcessHandler                          _process;
    std::list<std::string>                  _process_arguments;
    Environment                             _environment;
    zhttpd::api::size_t                        _request_content_length;
    std::map<std::string, std::string>      _executable_handler;
    std::string                             _script_name;
    std::string                             _extension;
    bool                                    _started;
    bool                                    _end;
    bool                                    _headers;

public:
    CGI(zhttpd::mod::POLICIES::MapConfigurationPolicy* manager);
    ~CGI();
    bool processRequest(zhttpd::api::event::Type event, request_t* request, buffer_t* buffer);
    bool isStarted() const;

private:
    bool        _processOnRequestData(request_t* request, buffer_t* buffer);
    bool        _processOnIdle(request_t* request, buffer_t* buffer);
    bool        _processOnEnd(request_t* request, buffer_t* buffer);
    void        _initialize(request_t* request);
    void        _fillEnvironment(request_t* request);
    void        _debug(request_t* request, buffer_t* buffer);
    bool        _checkContentLength(zhttpd::api::uint32_t request_content_length);
    bool        _checkExtension(request_t* request);
    void        _parseHeaders(request_t* request, buffer_t* buffer);
    void        _parseHeadersLine(request_t* request, std::string const& line);
    buffer_t*   _extractHeaders(request_t* request, buffer_t* buffer);


};

#endif /* !__CGI_HPP__ */
