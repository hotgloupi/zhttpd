
#ifndef __SSLNETWORKMANAGER_HPP__
# define __SSLNETWORKMANAGER_HPP__

# include <string>
# include <map>
# include <openssl/ssl.h>

# include "api/IModuleManager.hpp"
# include "api/constants.hpp"
# include "modules/common/AbstractManager.hpp"

# include "thread/Mutex.hpp"
# include "SSLNetwork.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class SSLNetworkManager : public StatefullManager<SSLNetwork>
        {
        public:
            SSLNetworkManager();
            ~SSLNetworkManager();
            virtual void addConfigurationEntry(std::string const& key, std::string const& value);
            virtual API::CATEGORY::Type getCategory() const;
            virtual bool isRequired(API::IRequest const&) const;
            virtual ZHTTPD::API::IModule* getInstance(bool in_response = false);
            virtual void releaseInstance(API::IModule* module);

            SSL_CTX* getCtx() const;

            void addSsl(API::socket_t fd, SSL* ssl);
            SSL* getSsl(API::socket_t fd);
            void delSsl(SSL* ssl);

        private:
            void _initSsl();
            static int _passwordCallback(char* buf, int num, int rwflag, void* _this);

            std::string _private_file;
            std::string _certif_file;
            std::string _password;

            SSL_CTX* _ctx;

            std::map<API::socket_t, SSL*> _ssl;
            Mutex _ssl_mutex;
        };
    }
}

#endif /* !__SSLNETWORKMANAGER_HPP__ */
