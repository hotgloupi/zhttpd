
#ifndef __RESPONSEJSONVIEW_HPP__
# define __RESPONSEJSONVIEW_HPP__

# include "ViewAdaptor.hpp"
# include "IJsonView.hpp"

namespace view
{
    class ResponseJsonView : public ViewAdaptor<IJsonView>
    {
    public:
        ResponseJsonView();
        virtual zhttpd::api::IBuffer* convert(IViewable const& object,
                                              zhttpd::api::IBufferManager& manager) const;
    };
}

#endif /* !__RESPONSEJSONVIEW_HPP__ */

