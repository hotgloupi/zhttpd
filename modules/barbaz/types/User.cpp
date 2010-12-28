
# include "User.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    DEFINE_ATTRIBUTE_CLASS(UserBase,
        NEW_FIELD(UserBase, id, zhttpd::api::uint64_t),
        NEW_FIELD(UserBase, fullname, std::string),
        NEW_FIELD(UserBase, email, std::string),
        NEW_FIELD(UserBase, auth_type, std::string),
        NEW_FIELD(UserBase, role, std::string),
        NEW_FIELD(UserBase, inscription_date, time_t),
        NEW_FIELD(UserBase, last_login_date, time_t),
    );

    User::User() : UserBase()
    {
        UserBase::_role = "user";
    }

    void User::set_plainpassword(std::string const& password)
    {
    }
}
