/**
 * @file IHTMLView.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IHTMLVIEW_HPP__
# define __IHTMLVIEW_HPP__

# include "ViewAdaptor.hpp"

namespace view
{

    class IHTMLView : public MakeViewAdaptor<IHTMLView>
    {
    public:
        virtual zhttpd::api::IBuffer* convert(IViewable const& obj,
                                              zhttpd::api::IBufferManager& manager) const
        {
            if (IViewAdaptor* view = this->getAdaptor(obj.getViewableTypeId()))
                return view->convert(obj, manager);
            return manager.allocate(
                "<h1>View Error</h1><p>Cannot view " +
                zhttpd::Logger::toString(obj.getViewableTypeId()) +
                " in HTML</p>"
            );
        }
        virtual std::string const& getContentType() const
        {
            static std::string _("text/html");
            return _;
        }
        virtual ~IHTMLView() {}
    };
}
#endif /* !__IHTMLVIEW_HPP__ */

