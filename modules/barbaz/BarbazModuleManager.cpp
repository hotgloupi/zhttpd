
#include "sqlite/Connection.hpp"
#include "BarbazModuleManager.hpp"
#include "traversal/Subscribe.hpp"
#include "traversal/Login.hpp"
#include "view/IJsonView.hpp"
#include "view/IHTMLView.hpp"

BarbazModuleManager::BarbazModuleManager() :
    manager_base_t("mod_barbaz", zhttpd::api::category::PROCESSING)
{
    this->_traversals["subscribe"] = new traversal::Subscribe(*this);
    this->_traversals["login"] = new traversal::Login(*this);
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

std::auto_ptr<db::IConnection> BarbazModuleManager::getNewDBConnection()
{
    LOG_INFO("Database connection to '" + this->_configuration["database"] + '"');
    return std::auto_ptr<db::IConnection>(sqlite::open(this->_configuration["database"].c_str()));
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
