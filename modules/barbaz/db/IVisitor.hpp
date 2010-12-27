/**
 * @file IVisitor.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IVISITOR_HPP__
# define __IVISITOR_HPP__

# include <stdexcept>

namespace db
{
    class IItem;
    class AttributeInt;
    class AttributeInt64;
    class AttributeUint64;
    class AttributeDouble;
    class AttributeFloat;
    class AttributeBool;
    class AttributeString;

    class IVisitor
    {
    public:
        virtual void visitInt(AttributeInt const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitInt(AttributeInt const&, IItem&)");}
        virtual void visitInt64(AttributeInt64 const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitInt64(AttributeInt64 const&, IItem&)");}
        virtual void visitUint64(AttributeUint64 const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitUint64(AttributeUint64 const&, IItem&)");}
        virtual void visitDouble(AttributeDouble const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitDouble(AttributeDouble const&, IItem&)");}
        virtual void visitFloat(AttributeFloat const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitFloat(AttributeFloat const&, IItem&)");}
        virtual void visitBool(AttributeBool const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitBool(AttributeBool const&, IItem&)");}
        virtual void visitString(AttributeString const&, IItem&)
        {throw std::runtime_error("Not implemented: IVisitor::visitString(AttributeString const&, IItem&)");}

        virtual void visitInt(AttributeInt const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitInt(AttributeInt const&, IItem const&)");}
        virtual void visitInt64(AttributeInt64 const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitInt64(AttributeInt64 const&, IItem const&)");}
        virtual void visitUint64(AttributeUint64 const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitUint64(AttributeUint64 const&, IItem const&)");}
        virtual void visitDouble(AttributeDouble const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitDouble(AttributeDouble const&, IItem const&)");}
        virtual void visitFloat(AttributeFloat const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitFloat(AttributeFloat const&, IItem const&)");}
        virtual void visitBool(AttributeBool const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitBool(AttributeBool const&, IItem const&)");}
        virtual void visitString(AttributeString const&, IItem const&)
        {throw std::runtime_error("Not implemented: IVisitor::visitString(AttributeString const&, IItem const&)");}

        virtual ~IVisitor() {}
    };
}

#endif /* !__IVISITOR_HPP__ */

