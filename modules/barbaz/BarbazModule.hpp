/**
 * @file BarbazModule.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * Barbaz module handle requests
 */

#ifndef __BARBAZMODULE_HPP__
# define __BARBAZMODULE_HPP__

# include "api/IModuleManager.hpp"
# include "api/IModule.hpp"

class BarbazModuleManager;

class BarbazModule : public zhttpd::api::IModule
{
private:
    BarbazModuleManager* _manager;

public:
    BarbazModule(zhttpd::api::IModuleManager* manager);
    virtual ~BarbazModule() {}
    bool processRequest(zhttpd::api::event::Type event,
                        zhttpd::api::IRequest* request,
                        zhttpd::api::IBuffer* buffer);
};

#endif /* !__BARBAZMODULE_HPP__ */

