
# include "UserAction.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    DEFINE_ATTRIBUTE_CLASS(UserAction,
        NEW_FIELD(UserAction, title, std::string),
        NEW_FIELD(UserAction, url, std::string),
        NEW_FIELD(UserAction, new_tab, bool)
    );
}

