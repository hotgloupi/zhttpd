
#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

# include <string>
# include <map>
# include <string>

# include "api/IModule.hpp"
# include "api/IRequest.hpp"
# include "api/IBufferManager.hpp"
# include "utils/SmartPtr.hpp"
# include "thread/RWLock.hpp"
# include "configuration/Configuration.hpp"

# include "Session.hpp"
# include "ModuleList.hpp"
# include "ModuleWrapper.hpp"
# include "RequestTasks.hpp"

# ifdef ZHTTPD_DEBUG
#  include "SafeBufferManager.hpp"
# endif

namespace ZHTTPD
{
    struct HTTPResponse
    {
        API::HTTP_CODE::Type code;
        char const* message;
    };

    class Request : public API::IRequest
    {
    private:
        typedef std::map<std::string, std::string> headers_t;

# ifdef ZHTTPD_DEBUG
        SafeBufferManager           _buffer_manager;
# else
        API::IBufferManager&        _buffer_manager;
# endif
        Session&                    _session;
        std::string                 _request_file_path;
        API::HTTP_METHOD::Type      _request_method;
        std::string                 _request_query;
        mutable headers_t           _request_headers;

        API::HTTP_CODE::Type        _response_code;
        std::string                 _response_msg;
        mutable headers_t           _response_headers;

        ModuleList*                 _modules;
        ModuleWrapper*              _current_module;
        RequestTasks*               _tasks;
        volatile bool               _queued;
        Mutex                       _queued_mutex;
        SmartPtr<Configuration>     _configuration;
        static const HTTPResponse   _responses[];

    public:
        Request(API::IBufferManager& buffer_manager,
                Session& session,
                SmartPtr<Configuration> configuration);
        virtual ~Request();
        void callLater(API::uint32_t ms);
        void giveData(API::IBuffer* buffer);
        void raiseError(API::HTTP_CODE::Type code, std::string error = "");
        void raiseEnd();
        void needWrite(API::IBuffer* buffer);
        API::ISession const& getSession();
        API::IBufferManager& getBufferManager();

        std::string const& getFilePath() const;
        void setFilePath(std::string const& path);
        void setRequestHeader(std::string const& key, std::string const& val);
        std::string const& getRequestHeader(std::string const& key) const;
        std::list<std::string const*> getRequestHeaderKeys() const;
        void setRequestMethod(API::HTTP_METHOD::Type method);
        API::HTTP_METHOD::Type getRequestMethod() const;
        void setRequestQuery(std::string const& query);
        std::string const& getRequestQuery() const;

        void setResponseCode(API::HTTP_CODE::Type code, std::string const& reason = "");
        API::HTTP_CODE::Type getResponseCode() const;
        std::string const& getResponseMessage() const;
        void setResponseHeader(std::string const& key, std::string const& val);
        std::string const& getResponseHeader(std::string const& key) const;
        std::list<std::string const*> getResponseHeaderKeys() const;

        // méthodes non-présentes dans l'API
        ModuleWrapper* append(API::IModuleManager& manager, API::IModule& module);
        ModuleWrapper* insertAfter(API::IModuleManager& manager, API::IModule& module);
        Session& getServerSession();
        ModuleWrapper* getInputModule();
        ModuleWrapper* getOutputModule();
        void processTask(API::EVENT::Type evt,
                         ModuleWrapper* module,
                         API::IBuffer* buf);
        bool hasPendingEvent();
        RequestTasks& getRequestTasks();
        bool isQueued();
        void setQueued(bool state);
        SmartPtr<Configuration> getConfiguration();

    };
}

#endif /* !__REQUEST_HPP__ */

