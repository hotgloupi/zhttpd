
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

using namespace ZHTTPD;


Request::Request(API::IBufferManager& buffer_manager,
                 Session& session,
                 SmartPtr<Configuration> configuration) :
    _buffer_manager(buffer_manager),
    _session(session),
    _request_file_path(),
    _request_method(API::HTTP_METHOD::UNDEFINED),
    _request_query(),
    _request_headers(),
    _response_code(API::HTTP_CODE::UNDEFINED),
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

void Request::callLater(API::uint32_t delay)
{
    assert(this->_current_module != 0);
    this->_tasks->addCallLaterTask(*this, *this->_current_module, delay);
}

void Request::giveData(API::IBuffer* buffer)
{
    assert(this->_current_module != 0);
    assert(this->_current_module->getNext() != 0);
    assert(buffer != 0);
    if (this->_current_module->getNext()->getNext() == 0)
    {
        this->_tasks->needWrite(buffer);
        return;
    }

    API::EVENT::Type evt = (
        this->_modules->isInResponsePart(this->_current_module) ?
        API::EVENT::ON_RESPONSE_DATA :
        API::EVENT::ON_REQUEST_DATA
    );
    this->_tasks->addTask(evt, *this, *this->_current_module->getNext(), buffer);
}

void Request::raiseError(API::HTTP_CODE::Type code, std::string str)
{
    assert(this->_current_module != 0);
    this->setResponseCode(code, str);
    API::IModuleManager* mod_error_manager = this->_configuration->getModuleManager("mod_error");
    assert(mod_error_manager != 0);
    API::IModule* mod_error = mod_error_manager->getInstance();
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

void Request::needWrite(API::IBuffer* buffer)
{
    assert(this->_current_module != 0);
    assert(this->_current_module == this->getOutputModule());
    this->_tasks->needWrite(buffer);
}

API::ISession const& Request::getSession()
{
    return this->_session;
}

API::IBufferManager& Request::getBufferManager()
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

void Request::setRequestMethod(API::HTTP_METHOD::Type method)
{
    this->_request_method = method;
}

API::HTTP_METHOD::Type Request::getRequestMethod() const
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

void Request::setResponseCode(API::HTTP_CODE::Type code, std::string const& reason)
{
    this->_response_code = code;
    if (reason.size() > 0)
    {
        this->_response_msg = reason;
    }
    else
    {
        for (API::size_t i = 0 ;
             Request::_responses[i].code != ZHTTPD::API::HTTP_CODE::UNDEFINED ; ++i)
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

API::HTTP_CODE::Type Request::getResponseCode() const
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


ModuleWrapper* Request::append(API::IModuleManager& manager, API::IModule& module)
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
   | EVENT::Type      |       Actions faites sur le serveur                  |
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
        evt == API::EVENT::ON_CAN_READ ? "ON_CAN_READ" : \
        evt == API::EVENT::ON_CAN_WRITE ? "ON_CAN_WRITE" : \
        evt == API::EVENT::ON_REQUEST_DATA ? "ON_REQUEST_DATA" : \
        evt == API::EVENT::ON_RESPONSE_DATA ? "ON_RESPONSE_DATA" : \
        evt == API::EVENT::ON_END ? "ON_END" : \
        evt == API::EVENT::ON_IDLE ? "ON_IDLE" : \
        evt == API::EVENT::ON_ERROR ? "ON_ERROR" : \
        "UNKNOWN !" \
    )


void Request::processTask(API::EVENT::Type evt,
                          ModuleWrapper* module,
                          API::IBuffer* buf)
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
        case API::EVENT::ON_CAN_READ:
        case API::EVENT::ON_CAN_WRITE:
            if (buf != 0)
                this->getBufferManager().release(buf);
            return;
            break;
        case API::EVENT::ON_REQUEST_DATA:
        case API::EVENT::ON_RESPONSE_DATA:
            this->getBufferManager().release(buf);
            this->_tasks->addEndTask(*this, *this->getInputModule());
            LOG_WARN(this->_current_module->getModuleManager().getName() + " did not process data event.");
            break;
        case API::EVENT::ON_END:
            this->raiseEnd();
            break;
        case API::EVENT::ON_ERROR:
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

ModuleWrapper* Request::insertAfter(API::IModuleManager& manager, API::IModule& module)
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
    {ZHTTPD::API::HTTP_CODE::ACCEPTED                     , "Accepted"},
    {ZHTTPD::API::HTTP_CODE::CONTINUE                     , "Continue"},
    {ZHTTPD::API::HTTP_CODE::SWITCHING_PROTOCOLS          , "Switching Protocols"},
    {ZHTTPD::API::HTTP_CODE::PROCESSING                   , "Processing"},
    {ZHTTPD::API::HTTP_CODE::OK                           , "OK"},
    {ZHTTPD::API::HTTP_CODE::CREATED                      , "Created"},
    {ZHTTPD::API::HTTP_CODE::ACCEPTED                     , "Accepted "},
    {ZHTTPD::API::HTTP_CODE::NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"},
    {ZHTTPD::API::HTTP_CODE::NO_CONTENT                   , "No Content"},
    {ZHTTPD::API::HTTP_CODE::RESET_CONTENT                , "Reset Content"},
    {ZHTTPD::API::HTTP_CODE::PARTIAL_CONTENT              , "Partial Content"},
    {ZHTTPD::API::HTTP_CODE::MULTI_STATUS                 , "Multi-Status"},
    {ZHTTPD::API::HTTP_CODE::CONTENT_DIFFERENT            , "Content Different"},
    {ZHTTPD::API::HTTP_CODE::MULTIPLE_CHOICES             , "Multiple Choices"},
    {ZHTTPD::API::HTTP_CODE::MOVED_PERMANENTLY            , "Moved Permanently"},
    {ZHTTPD::API::HTTP_CODE::FOUND                        , "Found"},
    {ZHTTPD::API::HTTP_CODE::SEE_OTHER                    , "See Other"},
    {ZHTTPD::API::HTTP_CODE::NOT_MODIFIED                 , "Not Modified"},
    {ZHTTPD::API::HTTP_CODE::USE_PROXY                    , "Use Proxy"},
    {ZHTTPD::API::HTTP_CODE::TEMPORARY_REDIRECT           , "Temporary Redirect"},
    {ZHTTPD::API::HTTP_CODE::TOO_MANY_REDIRECT            , "Too many Redirect"},
    {ZHTTPD::API::HTTP_CODE::BAD_REQUEST                  , "Bad Request"},
    {ZHTTPD::API::HTTP_CODE::UNAUTHORIZED                 , "Unauthorized"},
    {ZHTTPD::API::HTTP_CODE::PAYMENT_REQUIRED             , "Payment Required"},
    {ZHTTPD::API::HTTP_CODE::FORBIDDEN                    , "Forbidden"},
    {ZHTTPD::API::HTTP_CODE::NOT_FOUND                    , "Not Found"},
    {ZHTTPD::API::HTTP_CODE::METHOD_NOT_ALLOWED           , "Method Not Allowed"},
    {ZHTTPD::API::HTTP_CODE::NOT_ACCEPTABLE               , "Not Acceptable"},
    {ZHTTPD::API::HTTP_CODE::PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
    {ZHTTPD::API::HTTP_CODE::REQUEST_TIME_OUT             , "Request Time-out"},
    {ZHTTPD::API::HTTP_CODE::CONFLICT                     , "Conflict"},
    {ZHTTPD::API::HTTP_CODE::GONE                         , "Gone"},
    {ZHTTPD::API::HTTP_CODE::LENGTH_REQUIRED              , "Length Required"},
    {ZHTTPD::API::HTTP_CODE::PRECONDITION_FAILED          , "Precondition Failed"},
    {ZHTTPD::API::HTTP_CODE::REQUEST_ENTITY_TOO_LARGE     , "Request Entity Too Large"},
    {ZHTTPD::API::HTTP_CODE::REQUEST_URI_TOO_LONG         , "Request-URI Too Long"},
    {ZHTTPD::API::HTTP_CODE::UNSUPPORTED_MEDIA_TYPE       , "Unsupported Media Type"},
    {ZHTTPD::API::HTTP_CODE::REQUESTED_RANGE_UNSATISFIABLE, "Requested range unsatisfiable"},
    {ZHTTPD::API::HTTP_CODE::EXPECTATION_FAILED           , "Expectation failed"},
    {ZHTTPD::API::HTTP_CODE::UNPROCESSABLE_ENTITY         , "Unprocessable entity"},
    {ZHTTPD::API::HTTP_CODE::LOCKED                       , "Locked"},
    {ZHTTPD::API::HTTP_CODE::METHOD_FAILURE               , "Method failure"},
    {ZHTTPD::API::HTTP_CODE::UNORDERED_COLLECTION         , "Unordered Collection"},
    {ZHTTPD::API::HTTP_CODE::UPGRADE_REQUIRED             , "Upgrade Required"},
    {ZHTTPD::API::HTTP_CODE::INTERNAL_SERVER_ERROR        , "Internal Server Error"},
    {ZHTTPD::API::HTTP_CODE::NOT_IMPLEMENTED              , "Not Implemented"},
    {ZHTTPD::API::HTTP_CODE::BAD_GATEWAY                  , "Bad Gateway ou Proxy Error"},
    {ZHTTPD::API::HTTP_CODE::SERVICE_UNAVAILABLE          , "Service Unavailable"},
    {ZHTTPD::API::HTTP_CODE::GATEWAY_TIME_OUT             , "Gateway Time-out"},
    {ZHTTPD::API::HTTP_CODE::HTTP_VERSION_NOT_SUPPORTED   , "HTTP Version not supported"},
    {ZHTTPD::API::HTTP_CODE::INSUFFICIENT_STORAGE         , "Insufficient storage"},
    {ZHTTPD::API::HTTP_CODE::BANDWIDTH_LIMIT_EXCEEDED     , "Bandwidth Limit Exceeded"},
    {ZHTTPD::API::HTTP_CODE::UNDEFINED                    , "End Of Codes"},
};
