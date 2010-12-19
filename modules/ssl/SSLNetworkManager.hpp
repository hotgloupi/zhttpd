
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

namespace zhttpd
{
    namespace mod
    {
        class SSLNetworkManager : public StatefullManager<SSLNetwork>
        {
        public:
            SSLNetworkManager();
            ~SSLNetworkManager();
            virtual void addConfigurationEntry(std::string const& key, std::string const& value);
            virtual api::category::Type getCategory() const;
            virtual bool isRequired(api::IRequest const&) const;
            virtual zhttpd::api::IModule* getInstance(bool in_response = false);
            virtual void releaseInstance(api::IModule* module);

            SSL_CTX* getCtx() const;

            void addSsl(api::socket_t fd, SSL* ssl);
            SSL* getSsl(api::socket_t fd);
            void delSsl(SSL* ssl);

        private:
            void _initSsl();
            static int _passwordCallback(char* buf, int num, int rwflag, void* _this);

            std::string _private_file;
            std::string _certif_file;
            std::string _password;

            SSL_CTX* _ctx;

            std::map<api::socket_t, SSL*> _ssl;
            Mutex _ssl_mutex;
        };
    }
}

#endif /* !__SSLNETWORKMANAGER_HPP__ */
