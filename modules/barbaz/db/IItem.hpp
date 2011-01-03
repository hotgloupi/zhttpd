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
    protected:
        IItem(IItem const& other);
        IItem& operator =(IItem const& other);

    public:
        IItem();
        virtual void visitAll(IVisitor&);
        virtual void visitAll(IVisitor&) const;
        virtual ~IItem();
        virtual viewable_types::Type getViewableTypeId() const;
    };
}

#endif /* !__IITEM_HPP__ */

