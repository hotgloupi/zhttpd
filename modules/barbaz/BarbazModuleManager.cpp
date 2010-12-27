
#include "sqlite/Connection.hpp"
#include "BarbazModuleManager.hpp"
#include "traversal/Subscribe.hpp"
#include "view/IJsonView.hpp"
#include "view/IHTMLView.hpp"

BarbazModuleManager::BarbazModuleManager() :
    manager_base_t("mod_barbaz", zhttpd::api::category::PROCESSING)
{
    this->_traversals["subscribe"] = new traversal::Subscribe(*this);
    this->_default_view = new view::IHTMLView();
    this->_views["html"] = this->_default_view;
    this->_views["json"] = new view::IJsonView();
}

BarbazModuleManager::~BarbazModuleManager()
{
}

bool BarbazModuleManager::isRequired(zhttpd::api::IRequest const&) const
{
    return true;
}

db::IConnection* BarbazModuleManager::getNewDBConnection()
{
    LOG_INFO("Database connection to '" + this->_configuration["database"] + '"');
    return sqlite::open(this->_configuration["database"].c_str());
}

void BarbazModuleManager::releaseDBConnection(db::IConnection* conn)
{
    sqlite::close(conn);
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
