/**
 * @file IItem.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IITEM_HPP__
# define __IITEM_HPP__

# include <stdexcept>

# include "view/IViewable.hpp"

namespace db
{
    class IVisitor;

    class IItem : public view::IViewable
    {
    public:
        virtual void visitAll(IVisitor&) {throw std::runtime_error("Not implemented"); };
        virtual void visitAll(IVisitor&) const {throw std::runtime_error("Not implemented"); };
        virtual ~IItem() {}
        viewable_types::Type getViewableTypeId() const
        {
            return viewable_types::DB_ITEM;
        }
    };
}

#endif /* !__IITEM_HPP__ */

