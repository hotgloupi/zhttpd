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
# include "server/modules/common/AbstractProcessingModule.hpp"

class BarbazModuleManager;

class BarbazModule : public zhttpd::mod::AbstractProcessingModule<true>
{
private:
    BarbazModuleManager* _manager;

public:
    BarbazModule(zhttpd::api::IModuleManager* manager);
    virtual ~BarbazModule() {}
    virtual bool processOnRequestReady(zhttpd::api::IRequest& request,
                                       zhttpd::mod::PostData& post_data);
};

#endif /* !__BARBAZMODULE_HPP__ */

