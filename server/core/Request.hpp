
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

namespace zhttpd
{
    struct HTTPResponse
    {
        api::http_code::Type code;
        char const* message;
    };

    class Request : public api::IRequest
    {
    private:
        typedef std::map<std::string, std::string> headers_t;

# ifdef ZHTTPD_DEBUG
        SafeBufferManager           _buffer_manager;
# else
        api::IBufferManager&        _buffer_manager;
# endif
        Session&                    _session;
        std::string                 _request_file_path;
        api::http_method::Type      _request_method;
        std::string                 _request_query;
        mutable headers_t           _request_headers;

        api::http_code::Type        _response_code;
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
        Request(api::IBufferManager& buffer_manager,
                Session& session,
                SmartPtr<Configuration> configuration);
        virtual ~Request();
        void callLater(api::uint32_t ms);
        void giveData(api::IBuffer* buffer);
        void raiseError(api::http_code::Type code, std::string error = "");
        void raiseEnd();
        void needWrite(api::IBuffer* buffer);
        api::ISession const& getSession();
        api::IBufferManager& getBufferManager();

        std::string const& getFilePath() const;
        void setFilePath(std::string const& path);
        void setRequestHeader(std::string const& key, std::string const& val);
        std::string const& getRequestHeader(std::string const& key) const;
        std::list<std::string const*> getRequestHeaderKeys() const;
        void setRequestMethod(api::http_method::Type method);
        api::http_method::Type getRequestMethod() const;
        void setRequestQuery(std::string const& query);
        std::string const& getRequestQuery() const;

        void setResponseCode(api::http_code::Type code, std::string const& reason = "");
        api::http_code::Type getResponseCode() const;
        std::string const& getResponseMessage() const;
        void setResponseHeader(std::string const& key, std::string const& val);
        std::string const& getResponseHeader(std::string const& key) const;
        std::list<std::string const*> getResponseHeaderKeys() const;

        // méthodes non-présentes dans l'API
        ModuleWrapper* append(api::IModuleManager& manager, api::IModule& module);
        ModuleWrapper* insertAfter(api::IModuleManager& manager, api::IModule& module);
        Session& getServerSession();
        ModuleWrapper* getInputModule();
        ModuleWrapper* getOutputModule();
        void processTask(api::event::Type evt,
                         ModuleWrapper* module,
                         api::IBuffer* buf);
        bool hasPendingEvent();
        RequestTasks& getRequestTasks();
        bool isQueued();
        void setQueued(bool state);
        SmartPtr<Configuration> getConfiguration();

    };
}

#endif /* !__REQUEST_HPP__ */

