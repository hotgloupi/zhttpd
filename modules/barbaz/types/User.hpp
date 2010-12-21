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

struct User : public IViewable
{
    unsigned int id;
    std::string email;
    std::string fullname;
    std::string password;
    time_t inscription_date;
    time_t last_login_date;
    std::string role;
    bool confirmed;


    virtual viewable_types::Type getViewableTypeId() const
    {
        return viewable_types::USER;
    }
};

#endif /* !__USER_HPP__ */

