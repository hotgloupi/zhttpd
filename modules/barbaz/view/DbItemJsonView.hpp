/**
 * @file DbItemJsonView.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __DBITEMJSONVIEW_HPP__
# define __DBITEMJSONVIEW_HPP__

# include "ViewAdaptor.hpp"
# include "IJsonView.hpp"
# include "db/IItem.hpp"

class DbItemJsonView : public ViewAdaptor<IJsonView>
{
public:
    DbItemJsonView();
    virtual zhttpd::api::IBuffer* convert(IViewable const& object,
                                          zhttpd::api::IBufferManager& manager) const;
};
#endif /* !__DBITEMJSONVIEW_HPP__ */

