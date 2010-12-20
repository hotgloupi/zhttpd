/**
 * @file UserJsonView.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __USERJSONVIEW_HPP__
# define __USERJSONVIEW_HPP__

# include "ViewAdaptor.hpp"
# include "IJsonView.hpp"
# include "User.hpp"

class UserJsonView : public ViewAdaptor<IJsonView>
{
public:
    UserJsonView();
    virtual zhttpd::api::IBuffer* convert(IViewable const& object,
                                          zhttpd::api::IBufferManager& manager) const;
};


#endif /* !__USERJSONVIEW_HPP__ */

