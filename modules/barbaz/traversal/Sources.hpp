
#ifndef __SOURCESTRAVERSAL_HPP__
# define __SOURCESTRAVERSAL_HPP__

# include "ATraversal.hpp"

class BarbazModuleManager;

namespace traversal
{
    class Sources : public ATraversal<Sources>
    {
        BarbazModuleManager& _manager;

    public:
        Sources(BarbazModuleManager& manager);
        view::IViewable* mine(zhttpd::api::IRequest&, zhttpd::mod::PostData&);
    };
}
#endif /* !__SOURCES_HPP__ */

