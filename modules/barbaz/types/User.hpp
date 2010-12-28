
#ifndef __USER_HPP__
# define __USER_HPP__

# include <string>
# include <ctime>

# include "api/types.hpp"
# include "view/IViewable.hpp"
# include "db/Item.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    class UserBase : public db::Item<UserBase>
    {
        DECLARE_ATTRIBUTE_CLASS(UserBase);

        ATTR_GETSET(id, zhttpd::api::uint64_t);
        ATTR_GETSET(fullname, std::string);
        ATTR_GETSET(email, std::string);
        ATTR_GETSET(auth_type, std::string);
        ATTR_GETSET(role, std::string);
        ATTR_GETSET(inscription_date, time_t);
        ATTR_GETSET(last_login_date, time_t);
    };

    class User : public UserBase
    {
    public:
        User();
        virtual void set_plainpassword(std::string const& password);
    };
}

#endif /* !__USER_HPP__ */

