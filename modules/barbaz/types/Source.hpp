
#ifndef __SOURCE_HPP__
# define __SOURCE_HPP__

# include <string>

# include "db/Item.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    class Source : public db::Item<Source>
    {
        DECLARE_ATTRIBUTE_CLASS(Source);
        ATTR_GETSET(id, zhttpd::api::uint64_t);
        ATTR_GETSET(name, std::string);
        ATTR_GETSET(type, std::string);
    };
}

#endif /* !__SOURCE_HPP__ */

