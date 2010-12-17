
#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <list>
#include <utility>
#include <algorithm>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "api/IModuleManager.hpp"
#include "utils/Logger.hpp"
#include "utils/Path.hpp"

#include "CGI.hpp"
#include "Utils.hpp"
#include "Process.hpp"

zhttpd::api::size_t const  CGI::BUFFER_SIZE = 4096;

CGI::action_t const CGI::_actions[zhttpd::api::event::ON_ERROR] = {
    0,
    0,
    &CGI::_processOnRequestData,
    0,
    &CGI::_processOnIdle,
    &CGI::_processOnEnd,
    0
};

CGI::CGI(zhttpd::mod::policies::MapConfigurationPolicy* module_manager) :
        _module_manager(*module_manager)
{
    this->_started = false;
    this->_end = false;
    this->_headers = true;
}

CGI::~CGI()
{

}

bool CGI::processRequest(zhttpd::api::event::Type event, request_t* request, buffer_t* buffer)
{
    action_t action = CGI::_actions[event - 1];
    if (!this->_started)
    {
        this->_initialize(request);
        try
        {
            this->_process.create(this->_module_manager.getConfigurationValue(this->_extension),
                                  this->_process_arguments,
                                  this->_environment,
                                  zhttpd::Path(request->getFilePath()).getBasePath().c_str());
            this->_started = true;
        }
        catch (std::exception& e)
        {
            if (buffer != 0)
                request->getBufferManager().release(buffer);
            LOG_ERROR("CGI Error: " + e.what());
            request->raiseError(zhttpd::api::http_code::INTERNAL_SERVER_ERROR);
            return (true);
        }
    }
    if (action != 0)
        return (this->*action)(request, buffer);
    return (false);
}

bool CGI::isStarted() const
{
    return (this->_started);
}

bool                            CGI::_checkExtension(request_t* request)
{
    std::string::size_type idx = request->getFilePath().rfind('.');
    if(idx != std::string::npos)
    {
        this->_extension = request->getFilePath().substr(idx + 1);
        return (true);
    }
    else
        return (false);
}

void CGI::_initialize(request_t* request)
{
    this->_checkExtension(request);
    this->_method = request->getRequestMethod();
    this->_fillEnvironment(request);
    this->_environment.copyHeadersToEnvironment(request);
    this->_script_name = request->getFilePath();
    this->_process_arguments.push_back(this->_module_manager.getConfigurationValue(this->_extension));
    this->_process_arguments.push_back(this->_script_name);
}

bool CGI::_processOnRequestData(request_t *request, buffer_t *buffer)
{
    zhttpd::api::size_t nwrite = this->_process.write(buffer->getRawData(), buffer->getSize());
    if (nwrite < buffer->getSize())
    {
        LOG_ERROR("Lost post data, cannot give it to the child process");
    }
    request->getBufferManager().release(buffer);
    return (true);

}

bool            CGI::_processOnIdle(CGI::request_t* request, CGI::buffer_t*)
{
    buffer_t*   temp_buffer = 0;
    buffer_t*   to_send = 0;
    char        read_buffer[CGI::BUFFER_SIZE];

    zhttpd::api::size_t n_read = this->_process.read(read_buffer, CGI::BUFFER_SIZE - 1);
    if (n_read > 0)
    {
        read_buffer[n_read] = '\0';
        temp_buffer = request->getBufferManager().allocate(read_buffer, n_read);
        if (this->_headers == true)
        {
            to_send = this->_extractHeaders(request, temp_buffer);
            request->getBufferManager().release(temp_buffer);
        }
        else
            to_send = temp_buffer;
        if (to_send != 0)
        {
            request->giveData(to_send);
        }
        else
            LOG_DEBUG("Only headers found: " + read_buffer);

    }
    else
        LOG_DEBUG("Nothing to read");
    if (this->_end && n_read > 0)
        this->_end = false;
    if (this->_end)
    {
        if (request->getResponseHeader("Location") != "")
            request->setResponseCode(zhttpd::api::http_code::FOUND);
        request->raiseEnd();
        return true;
    }
    if (this->_process.getStatus() == PROCESS_STATUS::FINISHED)
    {
        LOG_DEBUG("Process done next step is ending");
        this->_end = true;
    }
    request->callLater(n_read == 0 ? 1 : 0);
    return (true);
}

bool CGI::_processOnEnd(CGI::request_t* request, buffer_t*)
{
    request->callLater();
    return (true);
}

void CGI::_fillEnvironment(request_t* request)
{
    char const* request_uri = request->getRequestQuery().c_str();
    std::size_t request_uri_size = request->getRequestQuery().size();
    char* path = new char[request_uri_size + 1];
    ::memcpy(path, request_uri, request_uri_size);
    path[request_uri_size] = '\0';
    std::size_t i = 0;
    while (path[i] != '\0' && path[i] != '?' && path[i] != '#')
        ++i;
    std::size_t query_pos = (path[i] == '?' ? i + 1 : i);
    path[i] = '\0';
    this->_environment.setEnvironmentVariable("GATEWAY_INTERAFCE", "CGI/1.1");
    this->_environment.setEnvironmentVariable("REQUEST_URI", request_uri);
    this->_environment.setEnvironmentVariable("SCRIPT_NAME", path);
    this->_environment.setEnvironmentVariable("SCRIPT_FILENAME", request->getFilePath().c_str());
    this->_environment.setEnvironmentVariable("SERVER_SOFTWARE", "ZHTTPD HTTPD");
    this->_environment.setEnvironmentVariable("SERVER_NAME", "zhttpd/0.1");
    this->_environment.setEnvironmentVariable("SERVER_PROTOCOL", "HTTP/1.1");
    this->_environment.setEnvironmentVariable("PATH_TRANSLATED", request->getFilePath().c_str());
    this->_environment.setEnvironmentVariable("SERVER_PORT",
                                              std::string(ss_itoa<char>(request->getSession().getPort())).c_str());
    this->_environment.setEnvironmentVariable("REDIRECT_STATUS", "200");
    if (this->_method == zhttpd::api::http_method::GET)
        this->_environment.setEnvironmentVariable("REQUEST_METHOD", "GET");
    else if (this->_method == zhttpd::api::http_method::POST)
        this->_environment.setEnvironmentVariable("REQUEST_METHOD", "POST");
    else
        request->raiseError(zhttpd::api::http_code::METHOD_NOT_ALLOWED);
    this->_environment.setEnvironmentVariable("QUERY_STRING", &request_uri[query_pos]);
    delete [] path;
}

bool CGI::_checkContentLength(zhttpd::api::uint32_t request_content_length)
{
    this->_request_content_length = request_content_length;
    return (true);
}

void CGI::_parseHeaders(request_t* request, buffer_t* headers)
{
    bool carriage_return = false;
    std::string line;

    for (zhttpd::api::size_t i = 0; i < headers->getSize(); ++i)
    {
        if (headers->getRawData()[i] == '\r')
            carriage_return = true;
        else if (headers->getRawData()[i] == '\n' && carriage_return)
        {
            this->_parseHeadersLine(request, line);
            line = "";
            carriage_return = false;
        }
        else
        {
            line.push_back(headers->getRawData()[i]);
            carriage_return = false;
        }
    }
}

void CGI::_parseHeadersLine(request_t* request, std::string const & line)
{
    if (!line.size())
        return;
    bool got_directive = false;
    bool got_separator = false;
    std::string directive;
    std::string params;
    std::string::const_iterator it = line.begin();
    std::string::const_iterator ite = line.end();
    for (; it != ite; ++it)
    {
        if (got_separator)
            params.push_back(*it);
        else if (got_directive)
            got_separator = true;
        else if (*it == ':')
            got_directive = true;
        else
            directive.push_back(*it);
    }
    request->setResponseHeader(directive.c_str(), params.c_str());
}

CGI::buffer_t* CGI::_extractHeaders(request_t* request, buffer_t* buffer)
{
    CGI::buffer_t* data = NULL;
    CGI::buffer_t* headers = NULL;
    unsigned int separator = 0;

    for (zhttpd::api::size_t i = 0; i < buffer->getSize(); ++i)
    {
        if (((separator == 0 || separator == 2) && buffer->getRawData()[i] == '\r') ||
            ((separator == 1 || separator == 3) && buffer->getRawData()[i] == '\n'))
            ++separator;
        else
            separator = 0;
        if (separator == 4)
        {
            headers = request->getBufferManager().allocate(buffer->getRawData(), i);
            this->_parseHeaders(request, headers);
            request->getBufferManager().release(headers);
            this->_headers = false;
            if (i + 1 < buffer->getSize())
            {
                data = request->getBufferManager().allocate(buffer->getRawData() + i + 1, buffer->getSize() - i - 1);
                return (data);
            }
        }
    }
    return (0);
}
