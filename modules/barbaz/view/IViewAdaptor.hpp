/**
 * @file IViewAdaptor.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IVIEWADAPTOR_HPP__
# define __IVIEWADAPTOR_HPP__

# include "api/IBuffer.hpp"
# include "api/IBufferManager.hpp"
# include "IViewable.hpp"

namespace view
{

    class IViewAdaptor
    {
    public:
        virtual zhttpd::api::IBuffer* convert(IViewable const& object,
                                              zhttpd::api::IBufferManager& manager) const = 0;
        virtual std::string const& getContentType() const = 0;
        virtual ~IViewAdaptor() {}
    };
}

#endif /* !__IVIEWADAPTOR_HPP__ */

