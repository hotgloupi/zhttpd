/**
 * @file User.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __USER_HPP__
# define __USER_HPP__

# include "IViewable.hpp"

struct User : public IViewable
{
    unsigned int id;

    virtual viewable_types::Type getViewableTypeId() const
    {
        return viewable_types::USER;
    }
};

#endif /* !__USER_HPP__ */

