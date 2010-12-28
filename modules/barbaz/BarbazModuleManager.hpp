/**
 * @file ModuleManager.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * Manager for barbaz module
 */

#ifndef __MODULEMANAGER_HPP__
# define __MODULEMANAGER_HPP__

# include <memory>

# include "server/modules/common/AbstractManager.hpp"
# include "db/IConnection.hpp"
# include "traversal/ITraversal.hpp"
# include "view/IViewAdaptor.hpp"
# include "BarbazModule.hpp"
# include "types/User.hpp"

typedef zhttpd::mod::StatefullManager<BarbazModule, zhttpd::mod::policies::MapConfigurationPolicy> manager_base_t;

class BarbazModuleManager : public manager_base_t
{
public:
    typedef std::map<std::string, traversal::ITraversal*> traversals_t;
private:
    typedef std::map<std::string, view::IViewAdaptor*> views_t;

private:
    traversals_t _traversals;
    views_t _views;
    view::IViewAdaptor* _default_view;

public:
    BarbazModuleManager();
    virtual ~BarbazModuleManager();
    virtual bool isRequired(zhttpd::api::IRequest const& request) const;
    std::auto_ptr<db::IConnection> getNewDBConnection() const;
    traversals_t const& getTraversals() const;
    view::IViewAdaptor const& getViewAdaptor(zhttpd::api::IRequest const&) const;
    std::auto_ptr<types::User> getUser(zhttpd::api::IRequest& req) const;
};

#endif /* !__MODULEMANAGER_HPP__ */

