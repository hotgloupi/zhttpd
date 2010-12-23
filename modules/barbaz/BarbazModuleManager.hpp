/**
 * @file ModuleManager.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * Manager for barbaz module
 */

#ifndef __MODULEMANAGER_HPP__
# define __MODULEMANAGER_HPP__

# include "server/modules/common/AbstractManager.hpp"
# include "BarbazModule.hpp"
# include "db/IConnection.hpp"

typedef zhttpd::mod::StatefullManager<BarbazModule, zhttpd::mod::policies::MapConfigurationPolicy> manager_base_t;

class BarbazModuleManager : public manager_base_t
{
public:
    BarbazModuleManager();
    virtual ~BarbazModuleManager();
    virtual bool isRequired(zhttpd::api::IRequest const& request) const;
    db::IConnection* getNewDBConnection();
    void releaseDBConnection(db::IConnection* conn);
};

#endif /* !__MODULEMANAGER_HPP__ */

