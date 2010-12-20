/**
 * @file IJsonView.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IJSONVIEW_HPP__
# define __IJSONVIEW_HPP__

# include "ViewAdapter.hpp"

class IJsonView : public MakeViewAdapter<IJsonView>
{
public:
    virtual zhttpd::api::IBuffer* convert(IViewable const& obj,
                                          zhttpd::api::IBufferManager& manager) const
    {
        if (IViewAdapter* view = this->getAdapter(obj.getViewableTypeId()))
            return view->convert(obj, manager);
        return manager.allocate("{\"error\": \"Cannot view this object in json\"}", 45);
    }
    virtual ~IJsonView() {}
};

#endif /* !__IJSONVIEW_HPP__ */

