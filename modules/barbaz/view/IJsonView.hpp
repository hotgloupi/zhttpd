/**
 * @file IJsonView.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IJSONVIEW_HPP__
# define __IJSONVIEW_HPP__

# include "ViewAdaptor.hpp"

namespace view
{
    class IJsonView : public MakeViewAdaptor<IJsonView>
    {
    public:
        virtual zhttpd::api::IBuffer* convert(IViewable const& obj,
                                              zhttpd::api::IBufferManager& manager) const
        {
            LOG_DEBUG("Try to convert into JSON");
            if (IViewAdaptor* view = this->getAdaptor(obj.getViewableTypeId()))
            {
                LOG_DEBUG("Found view !");
                return view->convert(obj, manager);
            }
            return manager.allocate("{\"error\": \"Cannot view this object in json\"}", 45);
        }
        virtual std::string const& getContentType() const
        {
            static std::string _("application/json");
            return _;
        }
        virtual ~IJsonView() {}
    };
}

#endif /* !__IJSONVIEW_HPP__ */

