
# include "User.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    DEFINE_ATTRIBUTE_CLASS(User,
        NEW_FIELD(User, id, unsigned int),
        NEW_FIELD(User, email, std::string),
        NEW_FIELD(User, full_name, std::string),
        NEW_FIELD(User, password, std::string),
        NEW_FIELD(User, inscription_date, time_t),
        NEW_FIELD(User, last_login_date, time_t),
        NEW_FIELD(User, role, std::string),
        NEW_FIELD(User, confirmed, bool)
    );
}
