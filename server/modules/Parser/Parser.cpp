
#include <cassert>
#include <sstream>
#include <string>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModuleManager.hpp"
#include "api/IModule.hpp"
#include "core/Request.hpp"
#include "config.hpp"

#include "Parser.hpp"
#include "ParserManager.hpp"


using namespace zhttpd::mod;

Parser::Parser(zhttpd::api::IModuleManager* manager) :
    _manager(dynamic_cast<ParserManager*>(manager)),
    _separator(0),
    _completed(false),
    _content_length(0),
    _received_length(0),
    _header_length(0),
    _request(0)
{
    assert(this->_manager != 0 && "Given manager instance is not a ParserManager");
}

Parser::~Parser()
{
    if (this->_request != 0 && this->_completed == false)
    {
        std::list<api::IBuffer*>::iterator it = this->_buffers.begin();
        std::list<api::IBuffer*>::iterator it_end = this->_buffers.end();
        for (; it != it_end; ++it)
        {
            this->_request->getBufferManager().release(*it);
        }
    }
}

void Parser::_parseLine(zhttpd::api::IRequest* request, std::string const& line) const
{
    if (!line.size())
        return;
    bool gotDirective = false;
    bool gotSeparator = false;
    std::string directive;
    std::string params;
    std::string::const_iterator it = line.begin();
    std::string::const_iterator itEnd = line.end();
    for (; it != itEnd; ++it)
        if (gotSeparator)
            params.push_back(*it);
        else if (gotDirective)
            gotSeparator = true;
        else if (*it == ':')
            gotDirective = true;
        else
            directive.push_back(*it);
    request->setRequestHeader(directive, params);
}

void Parser::_parseMethod(zhttpd::api::IRequest* request, std::string const& line) const
{
    bool gotMethod = false;
    std::string method;
    std::string path;
    std::string::const_iterator it = line.begin();
    std::string::const_iterator itEnd = line.end();
    for (; it != itEnd; ++it)
    {
        if (gotMethod)
        {
            if (*it == ' ')
                break;
            else
                path.push_back(*it);
        }
        else
        {
            if (*it == ' ')
                gotMethod = true;
            else
                method.push_back(*it);
        }
    }
    request->setRequestMethod(this->_manager->getMethod(method));
    request->setRequestQuery(path);
}

zhttpd::api::size_t Parser::_parseRequest(zhttpd::api::IRequest* request) const
{
    bool methodParsed = false;
    bool carriageReturn = false;
    std::string line;
    std::list<api::IBuffer*>::const_iterator it = this->_buffers.begin();
    std::list<api::IBuffer*>::const_iterator itEnd = this->_buffers.end();
    for (; it != itEnd; ++it)
    {
        for (api::size_t i = 0; i < (*it)->getSize(); ++i)
        {
            if ((*it)->getRawData()[i] == '\r')
                carriageReturn = true;
            else if ((*it)->getRawData()[i] == '\n' && carriageReturn)
            {
                if (methodParsed)
                    this->_parseLine(request, line);
                else
                {
                    this->_parseMethod(request, line);
                    methodParsed = true;
                }
                line = "";
                carriageReturn = false;
            }
            else
            {
                line.push_back((*it)->getRawData()[i]);
                carriageReturn = false;
            }
        }
        request->getBufferManager().release(*it);
    }
    request->setResponseHeader("Server", ZHTTPD_FULLNAME);
    api::size_t len = 0;
    std::stringstream ss;
    ss << request->getRequestHeader("Content-Length");
    ss >> len;
    return len;
}

void Parser::_giveData(zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer)
{
    request->giveData(buffer);
    std::string s(buffer->getRawData(), buffer->getSize());
    this->_received_length += buffer->getSize();
    if (this->_received_length >= this->_content_length)
        request->raiseEnd();
}

bool Parser::processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer)
{
    if (this->_request == 0)
        this->_request = request;
    assert(request == this->_request);
    if (event == api::event::ON_REQUEST_DATA)
    {
        if (this->_completed)
        {
            this->_giveData(request, buffer);
            return true;
        }
        for (api::size_t i = 0; i < buffer->getSize(); ++i)
        {
            if (((this->_separator == 0 || this->_separator == 2) && buffer->getRawData()[i] == '\r') ||
                ((this->_separator == 1 || this->_separator == 3) && buffer->getRawData()[i] == '\n'))
                ++this->_separator;
            else
                this->_separator = 0;
            if (this->_separator == 4)
            {
                api::IBuffer* next = 0;
                if (i + 1 < buffer->getSize())
                    next = request->getBufferManager().allocate(buffer->getRawData() + i + 1, buffer->getSize() - (i + 1));
                buffer->setSize(i + 1);
                this->_buffers.push_back(buffer);
                this->_completed = true;
                if ((this->_content_length = this->_parseRequest(request)) > 0 && next != 0)
                    this->_giveData(request, next);
                else
                    request->raiseEnd();
                return true;
            }
        }
        if (!this->_completed)
        {
            Request* req = dynamic_cast<Request*>(request);
            assert(req != 0);
            req->getRequestTasks().needRead();
        }
        this->_buffers.push_back(buffer);
        this->_header_length += buffer->getSize();
        if (this->_header_length > MAX_HEADER_SIZE)
        {
            this->_parseRequest(request);
            request->raiseEnd();
        }
        return true;
    }
    return false;
}

