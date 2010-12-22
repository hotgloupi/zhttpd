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
        virtual void visitInt(AttributeInt const&, IItem&)             {throw std::runtime_error("Not implemented");}
        virtual void visitInt64(AttributeInt64 const&, IItem&)         {throw std::runtime_error("Not implemented");}
        virtual void visitUint64(AttributeUint64 const&, IItem&)       {throw std::runtime_error("Not implemented");}
        virtual void visitDouble(AttributeDouble const&, IItem&)       {throw std::runtime_error("Not implemented");}
        virtual void visitFloat(AttributeFloat const&, IItem&)         {throw std::runtime_error("Not implemented");}
        virtual void visitBool(AttributeBool const&, IItem&)           {throw std::runtime_error("Not implemented");}
        virtual void visitString(AttributeString const&, IItem&)       {throw std::runtime_error("Not implemented");}

        virtual void visitInt(AttributeInt const&, IItem const&)       {throw std::runtime_error("Not implemented");}
        virtual void visitInt64(AttributeInt64 const&, IItem const&)   {throw std::runtime_error("Not implemented");}
        virtual void visitUint64(AttributeUint64 const&, IItem const&) {throw std::runtime_error("Not implemented");}
        virtual void visitDouble(AttributeDouble const&, IItem const&) {throw std::runtime_error("Not implemented");}
        virtual void visitFloat(AttributeFloat const&, IItem const&)   {throw std::runtime_error("Not implemented");}
        virtual void visitBool(AttributeBool const&, IItem const&)     {throw std::runtime_error("Not implemented");}
        virtual void visitString(AttributeString const&, IItem const&) {throw std::runtime_error("Not implemented");}

        virtual ~IVisitor() {}
    };
}

#endif /* !__IVISITOR_HPP__ */

