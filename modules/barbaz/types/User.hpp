/**
 * @file User.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __USER_HPP__
# define __USER_HPP__

# include <string>
# include <ctime>

# include "view/IViewable.hpp"
# include "db/Item.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    class User : public db::Item<User>
    {
        DECLARE_ATTRIBUTE_CLASS();

        ATTR_GETSET(id, unsigned int);
        ATTR_GETSET(email, std::string);
        ATTR_GETSET(full_name, std::string);
        ATTR_GETSET(password, std::string);
        ATTR_GETSET(inscription_date, time_t);
        ATTR_GETSET(last_login_date, time_t);
        ATTR_GETSET(role, std::string);
        ATTR_GETSET(confirmed, bool);
    };
}

#endif /* !__USER_HPP__ */

