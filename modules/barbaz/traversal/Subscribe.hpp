/**
 * @file Subscribe.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __SUBSCRIBE_HPP__
# define __SUBSCRIBE_HPP__

# include "ATraversal.hpp"

class BarbazModuleManager;

namespace traversal
{
    class Subscribe : public ATraversal<Subscribe>
    {
        BarbazModuleManager& _manager;

    public:
        Subscribe(BarbazModuleManager& manager);
        view::IViewable* new_account(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
    };
}

#endif /* !__SUBSCRIBE_HPP__ */

