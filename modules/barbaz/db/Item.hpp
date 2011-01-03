/**
 * @file Item.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ITEM_HPP__
# define __ITEM_HPP__

# include <iostream>

# include "Attributes.hpp"
# include "IVisitor.hpp"
# include "IItem.hpp"
# include "AttributesMacros.hpp"
# include "AttributeError.hpp"
# include "AttributeIterator.hpp"

namespace db
{
    template<typename SelfClass>
    class Item : public IItem
    {
    public:
        typedef AttributeIterator<SelfClass> iterator;

    public:
        IAttribute const* const* const _fields;
    public:
        Item() : _fields(SelfClass::__fields__) {}
        template<typename OtherClass>
        Item(OtherClass const& item) : _fields(OtherClass::__fields__)
        {
# ifdef ZHTTP_DEBUG
            assert(::strcmp(SelfClass::__name__, OtherClass::__name__) == 0 &&
                   "Wrong item copy");
# endif
            LOG_DEBUG("Copy " + std::string(SelfClass::__name__));
            OtherClass& self = dynamic_cast<OtherClass&>(*this);
            struct Visitor : public db::IVisitor
            {
                OtherClass& self;

                Visitor(OtherClass& self, IAttribute& self_attr) : self(self) {}
                void visitInt(db::AttributeInt const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
                void visitInt64(db::AttributeInt64 const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
                void visitUint64(db::AttributeUint64 const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
                void visitDouble(db::AttributeDouble const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
                void visitFloat(db::AttributeFloat const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
                void visitString(db::AttributeString const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
                void visitBool(db::AttributeBool const& attr, db::IItem const& i)
                {
                    attr.setValue(self, attr.getValue(i));
                }
            };

            item.visitAll(Visitor(self));
        }

        Item& operator=(Item const&);
        void visitAll(IVisitor& visitor)
        {
            unsigned int i = 0;
            while (i < SelfClass::__fields_len__)
            {
                this->_fields[i]->visit(visitor, *this);
                ++i;
            }
        }

        void visitAll(IVisitor& visitor) const
        {
            unsigned int i = 0;
            while (i != SelfClass::__fields_len__)
            {
                this->_fields[i]->visit(visitor, *this);
                ++i;
            }
        }

        iterator operator[](const char* key)
        {
            unsigned int i = 0;
            while (i != SelfClass::__fields_len__)
            {
                if (this->_fields[i]->getName() == key)
                {
                    return iterator(*this, i);
                }
                ++i;
            }
            throw AttributeError(
                "Attribute not found: '" + std::string(key) + "' in class '" +
                SelfClass::__name__ + "'."
            );
        }

        iterator begin()
        {
            return iterator(*this, 0);
        }

        iterator end()
        {
            return iterator(*this, SelfClass::__fields_len__);
        }
    };
}

// dumping whole item to stdout
//class Dumper : public IVisitor
//{
//    std::ostream& _o;
//public:
//    Dumper(std::ostream& o) : _o(o) {}
//    void visitInt(AttributeInt const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//    void visitInt64(AttributeInt64 const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//    void visitUint64(AttributeUint64 const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//    void visitDouble(AttributeDouble const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//    void visitFloat(AttributeFloat const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//    void visitString(AttributeString const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//    void visitBool(AttributeBool const& attr, IItem& i)
//    { this->_o << '\t' << attr.getName() << " : "<< attr.getValue(i) << std::endl; }
//} dumper(std::cout);

#endif /* !__ITEM_HPP__ */

