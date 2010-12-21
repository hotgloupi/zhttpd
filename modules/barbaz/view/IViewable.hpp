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
        USER = 0,
        MAX_TYPES = 1,
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
                case viewable_types::USER:
                    return "User";
                default:
                    return "Unkown viewable type!";
                }
            }
        };
    }
}

class IViewable
{
public:
    virtual viewable_types::Type getViewableTypeId() const = 0;
    virtual ~IViewable() {}
};


#endif /* !__IVIEWABLE_HPP__ */

