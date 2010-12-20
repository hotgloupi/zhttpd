/**
 * @file IViewAdapter.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IVIEWADAPTER_HPP__
# define __IVIEWADAPTER_HPP__

# include "api/IBuffer.hpp"
# include "api/IBufferManager.hpp"
# include "IViewable.hpp"

class IViewAdapter
{
public:
    virtual zhttpd::api::IBuffer* convert(IViewable const& object,
                                          zhttpd::api::IBufferManager& manager) const = 0;
};

#endif /* !__IVIEWADAPTER_HPP__ */

