/**
 * @file ITraversal.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ITRAVERSAL_HPP__
# define __ITRAVERSAL_HPP__

# include <list>
# include <string>

# include "server/modules/common/PostData.hpp"
# include "api/IRequest.hpp"
# include "view/IViewable.hpp"

namespace traversal
{
    class ITraversal
    {
    public:
        virtual view::IViewable* traverse(std::list<std::string>& path,
                                    zhttpd::api::IRequest& r,
                                    zhttpd::mod::PostData& d) = 0;
        virtual ~ITraversal() {}
    };
}

#endif /* !__ITRAVERSAL_HPP__ */

