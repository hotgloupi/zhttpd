/**
 * @file IAttribute.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IATTRIBUTE_HPP__
# define __IATTRIBUTE_HPP__

# include <string>

namespace db
{
    class IVisitor;
    class IItem;

    class IAttribute
    {
    public:
        virtual std::string const& getName() const = 0;
        virtual void visit(IVisitor& v, IItem& i) const = 0;
        virtual void visit(IVisitor& v, IItem const& i) const = 0;
        virtual void setDefaultValue(IItem& item) const = 0;
        virtual ~IAttribute() {}
    };
}

#endif /* !__IATTRIBUTE_HPP__ */

