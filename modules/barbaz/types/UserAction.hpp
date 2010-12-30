
#ifndef __USERACTION_HPP__
# define __USERACTION_HPP__

# include "db/Item.hpp"
# include "db/AttributesMacros.hpp"
# include "db/Attributes.hpp"

namespace types
{
    class UserAction  : public db::Item<UserAction>
    {
        DECLARE_ATTRIBUTE_CLASS(UserAction);

        ATTR_GETSET(title, std::string);
        ATTR_GETSET(url, std::string);
        ATTR_GETSET(new_tab, bool);

    public:
        UserAction(const char* title, const char* url, bool new_tab = true) :
            _title(title), _url(url), _new_tab(new_tab)
        {}
    };
}

#endif /* !__USERACTION_HPP__ */

