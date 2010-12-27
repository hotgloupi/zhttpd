/**
 * @file ItemListJsonView.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ITEMLISTJSONVIEW_HPP__
# define __ITEMLISTJSONVIEW_HPP__

# include "ViewAdaptor.hpp"
# include "IJsonView.hpp"

namespace view
{
    class ItemListJsonView : public ViewAdaptor<IJsonView>
    {
    public:
        ItemListJsonView();
        virtual zhttpd::api::IBuffer* convert(IViewable const& object,
                                              zhttpd::api::IBufferManager& manager) const;
    };
}

#endif /* !__ITEMLISTJSONVIEW_HPP__ */

