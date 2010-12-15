
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <stdexcept>

#include "SSLNetworkManager.hpp"
#include "SSLNetwork.hpp"

#include "utils/Logger.hpp"
#include "thread/ScopeLock.hpp"

#include "CryptoLock.hpp"

using namespace zhttpd;
using namespace zhttpd::mod;

SSLNetworkManager::SSLNetworkManager() :
    StatefullManager<SSLNetwork>("mod_ssl"),
    _ctx(0)
{
}

SSLNetworkManager::~SSLNetworkManager()
{
    ERR_free_strings();
    SSL_CTX_free(this->_ctx);
}

void SSLNetworkManager::addConfigurationEntry(std::string const& key, std::string const& value)
{
    if (key == "password")
        this->_password = value;
    else if (key == "private_file")
        this->_private_file = value;
    else if (key == "certif_file")
        this->_certif_file = value;
}

api::category::Type SSLNetworkManager::getCategory() const
{
    return api::category::INPUTOUTPUT;
}

bool SSLNetworkManager::isRequired(api::IRequest const&) const
{
    return true;
}

api::IModule* SSLNetworkManager::getInstance(bool in_response)
{
    if (this->_ctx == 0)
    {
        try
        {
            this->_initSsl();
        }
        catch (std::exception& e)
        {
            LOG_WARN("SSL : Init CTX failure : " + e.what());
            this->_ctx = 0;
            throw;
        }
    }
    return new SSLNetwork(this, in_response);
}

void SSLNetworkManager::releaseInstance(api::IModule* module)
{
    delete module;
}

SSL_CTX* SSLNetworkManager::getCtx() const
{
    return this->_ctx;
}

int SSLNetworkManager::_passwordCallback(char* buf, int num, int, void* _this_)
{
    SSLNetworkManager* this_ = reinterpret_cast<SSLNetworkManager*>(_this_);

    size_t size = num;
    if (size < this_->_password.size() + 1)
        return(0);

    ::strcpy(buf, this_->_password.c_str());

    return(this_->_password.size());
}

void SSLNetworkManager::_initSsl()
{
    CRYPTO_set_locking_callback(CryptoLock::staticLock);
#ifndef _WIN32
    CRYPTO_set_id_callback(CryptoLock::getId);
#endif

    CRYPTO_set_dynlock_create_callback(CryptoLock::createMutex);
    CRYPTO_set_dynlock_lock_callback(CryptoLock::lockMutex);
    CRYPTO_set_dynlock_destroy_callback(CryptoLock::destroyMutex);

    SSL_library_init();
    SSL_load_error_strings();

#ifdef WIN32
    const SSL_METHOD* meth = SSLv23_server_method();
#else
    SSL_METHOD* meth = SSLv23_server_method();
#endif
    if ((this->_ctx = SSL_CTX_new(meth)) == 0)
        throw std::runtime_error(Logger::toString("SSL_CTX_new failure: ") + ERR_error_string(ERR_get_error(), 0));

    if (SSL_CTX_use_certificate_chain_file(this->_ctx, this->_certif_file.c_str()) == 0)
        throw std::runtime_error(this->_certif_file + " certificate: " + ERR_error_string(ERR_get_error(), 0));

    SSL_CTX_set_default_passwd_cb(this->_ctx, SSLNetworkManager::_passwordCallback);
    SSL_CTX_set_default_passwd_cb_userdata(this->_ctx, this);
    if (SSL_CTX_use_PrivateKey_file(this->_ctx, this->_private_file.c_str(),SSL_FILETYPE_PEM) == 0)
        throw std::runtime_error(this->_private_file + " private key: " + ERR_error_string(ERR_get_error(), 0));

    int rrr[1024];
    srand(time(0));
    for (size_t i = 0 ; i < 1024 ; ++i)
        rrr[i] = rand();
    RAND_seed(rrr, 1024 * sizeof(int));
}

void SSLNetworkManager::addSsl(api::socket_t fd, SSL* ssl)
{
    ScopeLock lock(this->_ssl_mutex);
    this->_ssl[fd] = ssl;
}

SSL* SSLNetworkManager::getSsl(api::socket_t fd)
{
    ScopeLock lock(this->_ssl_mutex);
    if (this->_ssl.find(fd) != this->_ssl.end())
        return this->_ssl[fd];
    return 0;
}

void SSLNetworkManager::delSsl(SSL* ssl)
{
    ScopeLock lock(this->_ssl_mutex);
    for (std::map<api::socket_t, SSL*>::iterator it = this->_ssl.begin(),
         ite = this->_ssl.end() ; it != ite ; ++it)
    {
        if (it->second == ssl)
        {
            this->_ssl.erase(it);
            break;
        }
    }
}
