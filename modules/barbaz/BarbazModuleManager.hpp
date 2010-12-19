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

class BarbazModuleManager : public zhttpd::mod::StatefullManager<BarbazModule>
{
public:
    BarbazModuleManager();
    virtual ~BarbazModuleManager();
    virtual bool isRequired(zhttpd::api::IRequest const& request) const;
};

#endif /* !__MODULEMANAGER_HPP__ */

