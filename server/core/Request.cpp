
#include <cassert>
#include <stdexcept>

#include "utils/Logger.hpp"
#include "utils/macros.hpp"

#include "Request.hpp"
#include "TaskManager.hpp"
#include "RequestManager.hpp"
#include "SessionManager.hpp"
#include "RequestTasks.hpp"

#include "modules/ModError/ModErrorManager.hpp"

using namespace zhttpd;


Request::Request(api::IBufferManager& buffer_manager,
                 Session& session,
                 SmartPtr<Configuration> configuration) :
    _buffer_manager(buffer_manager),
    _session(session),
    _request_file_path(),
    _request_method(api::http_method::UNDEFINED),
    _request_query(),
    _request_headers(),
    _response_code(api::http_code::UNDEFINED),
    _response_msg(),
    _response_headers(),
    _modules(new ModuleList()),
    _current_module(0),
    _tasks(0),
    _queued(false),
    _queued_mutex(),
    _configuration(configuration)
{
    this->_tasks = new RequestTasks(*this);
    StatsManager::getInstance()->addRequest();
}

Request::~Request()
{
    ZHTTPD_DELETE(this->_tasks);
    ZHTTPD_DELETE(this->_modules);
    SessionManager::getInstance()->endSession(this->_session);
}

void Request::callLater(api::uint32_t delay)
{
    assert(this->_current_module != 0);
    this->_tasks->addCallLaterTask(*this, *this->_current_module, delay);
}

void Request::giveData(api::IBuffer* buffer)
{
    assert(this->_current_module != 0);
    assert(this->_current_module->getNext() != 0);
    assert(buffer != 0);
    if (this->_current_module->getNext()->getNext() == 0)
    {
        this->_tasks->needWrite(buffer);
        return;
    }

    api::event::Type evt = (
        this->_modules->isInResponsePart(this->_current_module) ?
        api::event::ON_RESPONSE_DATA :
        api::event::ON_REQUEST_DATA
    );
    this->_tasks->addTask(evt, *this, *this->_current_module->getNext(), buffer);
}

void Request::raiseError(api::http_code::Type code, std::string str)
{
    assert(this->_current_module != 0);
    this->setResponseCode(code, str);
    api::IModuleManager* mod_error_manager = this->_configuration->getModuleConfiguration("mod_error").getModuleManager();
    assert(mod_error_manager != 0);
    api::IModule* mod_error = mod_error_manager->getInstance();
    assert(mod_error != 0);
    ModuleWrapper* wrapper = this->_modules->setProcessingModule(*mod_error_manager, *mod_error);
    this->_tasks->reset();
    if (this->_tasks->hasWork())
    {
        LOG_ERROR("Tasks left !");
        this->_tasks->reset();
    }
    this->_tasks->addEndTask(*this, *wrapper);
}

void Request::raiseEnd()
{
    assert(this->_current_module != 0);
    ModuleWrapper* next = this->_current_module->getNext();
    if (next != 0)
    {
        this->_tasks->addEndTask(*this, *next);
    }
}

void Request::needWrite(api::IBuffer* buffer)
{
    assert(this->_current_module != 0);
    assert(this->_current_module == this->getOutputModule());
    this->_tasks->needWrite(buffer);
}

api::ISession const& Request::getSession()
{
    return this->_session;
}

api::IBufferManager& Request::getBufferManager()
{
    return this->_buffer_manager;
}


std::string const& Request::getFilePath() const
{
    return this->_request_file_path;
}

void Request::setFilePath(std::string const& path)
{
    this->_request_file_path = path;
}

void Request::setRequestHeader(std::string const& key, std::string const& val)
{
    LOG_WARN("HEADER " + key + ": " + val);
    this->_request_headers[key] = val;
}

std::string const& Request::getRequestHeader(std::string const& key) const
{
    if (this->_request_headers.find(key) == this->_request_headers.end())
    {
        this->_request_headers[key] = "";
    }
    return this->_request_headers[key];
}

std::list<std::string const*> Request::getRequestHeaderKeys() const
{
    Request::headers_t::const_iterator it = this->_request_headers.begin();
    Request::headers_t::const_iterator it_end = this->_request_headers.end();
    std::list<std::string const*> res;
    for (; it != it_end; ++it)
        res.push_back(&it->first);
    return res;
}

void Request::setRequestMethod(api::http_method::Type method)
{
    this->_request_method = method;
}

api::http_method::Type Request::getRequestMethod() const
{
    return this->_request_method;
}

void Request::setRequestQuery(std::string const& query)
{
    this->_request_query = query;
}

std::string const& Request::getRequestQuery() const
{
    return this->_request_query;
}

void Request::setResponseCode(api::http_code::Type code, std::string const& reason)
{
    this->_response_code = code;
    if (reason.size() > 0)
    {
        this->_response_msg = reason;
    }
    else
    {
        for (api::size_t i = 0 ;
             Request::_responses[i].code != zhttpd::api::http_code::UNDEFINED ; ++i)
        {
            if (Request::_responses[i].code == code)
            {
                this->_response_msg = Request::_responses[i].message;
                return ;
            }
        }
        this->_response_msg = "Undefined HTTP code";
    }
}

api::http_code::Type Request::getResponseCode() const
{
    return this->_response_code;
}

std::string const& Request::getResponseMessage() const
{
    return this->_response_msg;
}

void Request::setResponseHeader(std::string const& key, std::string const& val)
{
    this->_response_headers[key] = val;
}

std::string const& Request::getResponseHeader(std::string const& key) const
{
    if (this->_response_headers.find(key) == this->_response_headers.end())
    {
        this->_response_headers[key] = "";
    }
    return this->_response_headers[key];
}

std::list<std::string const*> Request::getResponseHeaderKeys() const
{
    Request::headers_t::const_iterator it = this->_response_headers.begin();
    Request::headers_t::const_iterator it_end = this->_response_headers.end();
    std::list<std::string const*> res;
    for (; it != it_end; ++it)
        res.push_back(&it->first);
    return res;
}


ModuleWrapper* Request::append(api::IModuleManager& manager, api::IModule& module)
{
    return this->_modules->append(manager, module);
}

Session& Request::getServerSession()
{
    return this->_session;
}

ModuleWrapper* Request::getInputModule()
{
    return this->_modules->head();
}

ModuleWrapper* Request::getOutputModule()
{
    return this->_modules->tail();
}
/*
   ___________________________________________________________________________
   | event::Type      |       Actions faites sur le serveur                  |
   --------------------------------------------------------------------------|
   | ON_CAN_READ      | Met fin à la requête                                 |
   --------------------------------------------------------------------------|
   |                  | Peut choisir de mettre fin à la requête.             |
   | ON_CAN_WRITE     | Supprime le buffer passé en argument.                |
   --------------------------------------------------------------------------|
   |                  | Le serveur peut arrêter de donner ON_CAN_READ au     |
   | ON_REQUEST_DATA  | module d'entrée et lui donner ON_END.                |
   |                  | Supprime le buffer passé en argument.                |
   --------------------------------------------------------------------------|
   |                  | Le serveur peut arrêter de donner ON_CAN_READ au     |
   | ON_RESPONSE_DATA | module d'entrée et lui donner ON_END.                |
   |                  | Supprime le buffer passé en argument.                |
   --------------------------------------------------------------------------|
   | ON_IDLE          | Donne ON_END à ce même module.                       |
   --------------------------------------------------------------------------|
   | ON_END           | Redonne ON_END au prochain module.                   |
   --------------------------------------------------------------------------|
   | ON_ERROR         | Redonne ON_ERROR au prochain module.                 |
   |_________________________________________________________________________|
 */

#define EVENT_NAME(evt) (\
        evt == api::event::ON_CAN_READ ? "ON_CAN_READ" : \
        evt == api::event::ON_CAN_WRITE ? "ON_CAN_WRITE" : \
        evt == api::event::ON_REQUEST_DATA ? "ON_REQUEST_DATA" : \
        evt == api::event::ON_RESPONSE_DATA ? "ON_RESPONSE_DATA" : \
        evt == api::event::ON_END ? "ON_END" : \
        evt == api::event::ON_IDLE ? "ON_IDLE" : \
        evt == api::event::ON_ERROR ? "ON_ERROR" : \
        "UNKNOWN !" \
    )


void Request::processTask(api::event::Type evt,
                          ModuleWrapper* module,
                          api::IBuffer* buf)
{
    if (this->_current_module != 0)
    {
        if (buf != 0)
            this->getBufferManager().release(buf);
        throw std::runtime_error("Request chain has been broken");
    }
#ifdef ZHTTPD_DEBUG
    std::string name = module->getModuleManager().getName();
#endif
    LOG_DEBUG("> "+Logger::toString(this) + " " + name + " with " + EVENT_NAME(evt));
    this->_current_module = module;
    if (module->getModule().processRequest(evt, this, buf) == false)
    {
        switch (evt)
        {
        case api::event::ON_CAN_READ:
        case api::event::ON_CAN_WRITE:
            if (buf != 0)
                this->getBufferManager().release(buf);
            return;
            break;
        case api::event::ON_REQUEST_DATA:
        case api::event::ON_RESPONSE_DATA:
            this->getBufferManager().release(buf);
            this->_tasks->addEndTask(*this, *this->getInputModule());
            LOG_WARN(this->_current_module->getModuleManager().getName() + " did not process data event.");
            break;
        case api::event::ON_END:
            this->raiseEnd();
            break;
        case api::event::ON_ERROR:
            LOG_WARN("ON_ERROR event should not be received");
            break;
        default:
            LOG_ERROR("Cannot process default operation for unknown event");
            break;
        }
    }
    this->_current_module = 0;
    LOG_DEBUG("< "+Logger::toString(this) + " " + name + " with " + EVENT_NAME(evt));
}

#undef EVENT_NAME

ModuleWrapper* Request::insertAfter(api::IModuleManager& manager, api::IModule& module)
{
    ModuleWrapper *next = this->_current_module->getNext();
    this->_current_module->setNext(0);
    ModuleWrapper *rnext = new ModuleWrapper(manager, module);
    this->_current_module->setNext(rnext);
    rnext->setNext(next);
    return rnext;
}

RequestTasks& Request::getRequestTasks()
{
    return *this->_tasks;
}


bool Request::isQueued()
{
//    ZHTTPD_LOCK(this->_queued_mutex);
    bool res = this->_queued;
//    ZHTTPD_UNLOCK(this->_queued_mutex);
    return res;
}
void Request::setQueued(bool state)
{
  //  ZHTTPD_LOCK(this->_queued_mutex);
    this->_queued = state;
   // ZHTTPD_UNLOCK(this->_queued_mutex);
}


SmartPtr<Configuration> Request::getConfiguration()
{
    return this->_configuration;
}


const HTTPResponse Request::_responses[] = {
    {zhttpd::api::http_code::ACCEPTED                     , "Accepted"},
    {zhttpd::api::http_code::CONTINUE                     , "Continue"},
    {zhttpd::api::http_code::SWITCHING_PROTOCOLS          , "Switching Protocols"},
    {zhttpd::api::http_code::PROCESSING                   , "Processing"},
    {zhttpd::api::http_code::OK                           , "OK"},
    {zhttpd::api::http_code::CREATED                      , "Created"},
    {zhttpd::api::http_code::ACCEPTED                     , "Accepted "},
    {zhttpd::api::http_code::NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"},
    {zhttpd::api::http_code::NO_CONTENT                   , "No Content"},
    {zhttpd::api::http_code::RESET_CONTENT                , "Reset Content"},
    {zhttpd::api::http_code::PARTIAL_CONTENT              , "Partial Content"},
    {zhttpd::api::http_code::MULTI_STATUS                 , "Multi-Status"},
    {zhttpd::api::http_code::CONTENT_DIFFERENT            , "Content Different"},
    {zhttpd::api::http_code::MULTIPLE_CHOICES             , "Multiple Choices"},
    {zhttpd::api::http_code::MOVED_PERMANENTLY            , "Moved Permanently"},
    {zhttpd::api::http_code::FOUND                        , "Found"},
    {zhttpd::api::http_code::SEE_OTHER                    , "See Other"},
    {zhttpd::api::http_code::NOT_MODIFIED                 , "Not Modified"},
    {zhttpd::api::http_code::USE_PROXY                    , "Use Proxy"},
    {zhttpd::api::http_code::TEMPORARY_REDIRECT           , "Temporary Redirect"},
    {zhttpd::api::http_code::TOO_MANY_REDIRECT            , "Too many Redirect"},
    {zhttpd::api::http_code::BAD_REQUEST                  , "Bad Request"},
    {zhttpd::api::http_code::UNAUTHORIZED                 , "Unauthorized"},
    {zhttpd::api::http_code::PAYMENT_REQUIRED             , "Payment Required"},
    {zhttpd::api::http_code::FORBIDDEN                    , "Forbidden"},
    {zhttpd::api::http_code::NOT_FOUND                    , "Not Found"},
    {zhttpd::api::http_code::METHOD_NOT_ALLOWED           , "Method Not Allowed"},
    {zhttpd::api::http_code::NOT_ACCEPTABLE               , "Not Acceptable"},
    {zhttpd::api::http_code::PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
    {zhttpd::api::http_code::REQUEST_TIME_OUT             , "Request Time-out"},
    {zhttpd::api::http_code::CONFLICT                     , "Conflict"},
    {zhttpd::api::http_code::GONE                         , "Gone"},
    {zhttpd::api::http_code::LENGTH_REQUIRED              , "Length Required"},
    {zhttpd::api::http_code::PRECONDITION_FAILED          , "Precondition Failed"},
    {zhttpd::api::http_code::REQUEST_ENTITY_TOO_LARGE     , "Request Entity Too Large"},
    {zhttpd::api::http_code::REQUEST_URI_TOO_LONG         , "Request-URI Too Long"},
    {zhttpd::api::http_code::UNSUPPORTED_MEDIA_TYPE       , "Unsupported Media Type"},
    {zhttpd::api::http_code::REQUESTED_RANGE_UNSATISFIABLE, "Requested range unsatisfiable"},
    {zhttpd::api::http_code::EXPECTATION_FAILED           , "Expectation failed"},
    {zhttpd::api::http_code::UNPROCESSABLE_ENTITY         , "Unprocessable entity"},
    {zhttpd::api::http_code::LOCKED                       , "Locked"},
    {zhttpd::api::http_code::METHOD_FAILURE               , "Method failure"},
    {zhttpd::api::http_code::UNORDERED_COLLECTION         , "Unordered Collection"},
    {zhttpd::api::http_code::UPGRADE_REQUIRED             , "Upgrade Required"},
    {zhttpd::api::http_code::INTERNAL_SERVER_ERROR        , "Internal Server Error"},
    {zhttpd::api::http_code::NOT_IMPLEMENTED              , "Not Implemented"},
    {zhttpd::api::http_code::BAD_GATEWAY                  , "Bad Gateway ou Proxy Error"},
    {zhttpd::api::http_code::SERVICE_UNAVAILABLE          , "Service Unavailable"},
    {zhttpd::api::http_code::GATEWAY_TIME_OUT             , "Gateway Time-out"},
    {zhttpd::api::http_code::HTTP_VERSION_NOT_SUPPORTED   , "HTTP Version not supported"},
    {zhttpd::api::http_code::INSUFFICIENT_STORAGE         , "Insufficient storage"},
    {zhttpd::api::http_code::BANDWIDTH_LIMIT_EXCEEDED     , "Bandwidth Limit Exceeded"},
    {zhttpd::api::http_code::UNDEFINED                    , "End Of Codes"},
};
