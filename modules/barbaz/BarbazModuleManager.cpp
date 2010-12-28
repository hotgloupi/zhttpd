
#include "sqlite/Connection.hpp"
#include "BarbazModuleManager.hpp"
#include "traversal/Subscribe.hpp"
#include "traversal/Login.hpp"
#include "traversal/User.hpp"
#include "view/IJsonView.hpp"
#include "view/IHTMLView.hpp"
#include "broker/UsersBroker.hpp"

BarbazModuleManager::BarbazModuleManager() :
    manager_base_t("mod_barbaz", zhttpd::api::category::PROCESSING)
{
    this->_traversals["subscribe"] = new traversal::Subscribe(*this);
    this->_traversals["login"] = new traversal::Login(*this);
    this->_traversals["user"] = new traversal::User(*this);
    this->_default_view = new view::IHTMLView();
    this->_views["html"] = this->_default_view;
    this->_views["json"] = new view::IJsonView();
}

BarbazModuleManager::~BarbazModuleManager()
{
    if (this->_traversals.size() > 0)
    {
        traversals_t::iterator it = this->_traversals.begin(), end = this->_traversals.end();
        for (; it != end; ++it)
            delete (*it).second;
        this->_traversals.clear();
    }
    if (this->_views.size() > 0)
    {
        views_t::iterator it = this->_views.begin(), end = this->_views.end();
        for (; it != end; ++it)
            delete (*it).second;
        this->_views.clear();
    }
}

bool BarbazModuleManager::isRequired(zhttpd::api::IRequest const&) const
{
    return true;
}

std::auto_ptr<db::IConnection> BarbazModuleManager::getNewDBConnection() const
{
    static const char* path = 0;
    if (path == 0)
    {
        std::map<std::string, std::string>::const_iterator it = this->_configuration.find("database");
        if (it == this->_configuration.end())
            throw std::runtime_error("No database path was set in configuration");
        path = (*it).second.c_str();
    }
//    LOG_INFO("Database connection to '" + this->_configuration["database"] + '"');

    return std::auto_ptr<db::IConnection>(sqlite::open(path));
}

std::auto_ptr<types::User> BarbazModuleManager::getUser(zhttpd::api::IRequest& req) const
{
    std::string const& cookie = req.getRequestHeader("Cookie");
    std::string hash;
    std::size_t start = cookie.find("hash="), end;
    if (start == std::string::npos)
        return std::auto_ptr<types::User>(0);
    end = cookie.find_first_not_of("0123456789abcdef");
    hash = cookie.substr(start + 5, end - start - 5);
    if (hash.size() == 0)
        return std::auto_ptr<types::User>(0);
    types::User* user = broker::UsersBroker::getUserFromHash(*this->getNewDBConnection(), hash);
    return std::auto_ptr<types::User>(user);
}

BarbazModuleManager::traversals_t const& BarbazModuleManager::getTraversals() const
{
    return this->_traversals;
}

view::IViewAdaptor const& BarbazModuleManager::getViewAdaptor(zhttpd::api::IRequest const& req) const
{
    std::string const& query = req.getRequestQuery();
    std::size_t dot = query.find('.');
    std::string ext;

    if (dot != std::string::npos)
    {
        std::size_t end = query.find_first_of("/#?\\", dot);
        ext = query.substr(dot + 1, end);
    }
    if (ext.size() > 0)
    {
        views_t::const_iterator it = this->_views.find(ext);
        if (it != this->_views.end())
            return *((*it).second);
    }
    return *this->_default_view;
}
