/**
 * @file IViewable.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IVIEWABLE_HPP__
# define __IVIEWABLE_HPP__

# include "server/utils/Logger.hpp"

namespace viewable_types
{
    enum Type
    {
        DB_ITEM = 0,
        DB_ITEMS = 1,
        MAX_TYPES = 2,
    };
}

namespace zhttpd
{
    namespace LOGGER
    {
        template<>
        struct Stringify<viewable_types::Type>
        {
            static std::string toString(viewable_types::Type const& type)
            {
                switch (type)
                {
                case viewable_types::DB_ITEM:
                    return "db::Item";
                case viewable_types::DB_ITEMS:
                    return "db::ItemList";
                default:
                    return "Unkown viewable type!";
                }
            }
        };
    }
}

namespace view
{
    class IViewable
    {
    public:
        virtual viewable_types::Type getViewableTypeId() const = 0;
        virtual ~IViewable() {}
    };
}

#endif /* !__IVIEWABLE_HPP__ */

