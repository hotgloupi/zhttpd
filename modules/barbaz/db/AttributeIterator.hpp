/**
 * @file AttributeIterator.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ATTRIBUTEITERATOR_HPP__
# define __ATTRIBUTEITERATOR_HPP__

# include "server/utils/Logger.hpp"

namespace db
{
    template<typename T> class Item;

    template<typename T> class AttributeIterator
    {
    private:
        Item<T>* _item;
        unsigned int _index;
        IAttribute const* _attr;

    public:
        AttributeIterator(Item<T>& item, unsigned int index) :
            _item(&item), _index(index),
            _attr(index < T::__fields_len__ ? T::__fields__[index] : 0)
        {
        }
        AttributeIterator(AttributeIterator const& it) :
            _item(it._item), _index(it._index), _attr(it._attr)
        {
        }
        AttributeIterator& operator =(AttributeIterator const& it)
        {
            if (&it != this)
            {
                this->_item = it._item;
                this->_index = it._index;
                this->_attr = it._attr;
            }
            return *this;
        }
        AttributeIterator& operator ++() // prefix
        {
            if (this->_attr == 0)
                throw std::runtime_error(
                    "index " + zhttpd::Logger::toString(this->_index) +
                    " is out of range (" +
                    zhttpd::Logger::toString(T::__fields_len__) + ")");
            this->_index += 1;
            if (this->_index < T::__fields_len__)
                this->_attr = T::__fields__[this->_index];
            else
                this->_attr = 0;
            return *this;
        }

        AttributeIterator operator ++(int) // postfix
        {
            if (this->_attr == 0)
                throw std::runtime_error("index " + zhttpd::Logger::toString(this->_index) + " is out of range");
            this->_index += 1;
            if (this->_index < T::__fields_len__)
                this->_attr = T::__fields__[this->_index];
            else
                this->_attr = 0;
            return *this;
        }

        bool operator ==(AttributeIterator const& it) const
        {
            return this->_index == it._index && this->_item == it._item;
        }

        bool operator !=(AttributeIterator const& it) const
        {
            return this->_index != it._index || this->_item != it._item;
        }

        std::string const& key() const
        {
            if (this->_attr == 0)
                throw std::runtime_error(
                    "index " + zhttpd::Logger::toString(this->_index) +
                    " is out of range (" +
                    zhttpd::Logger::toString(T::__fields_len__) + ")");
            return this->_attr->getName();
        }

        template<typename FieldType>
        typename pc_const_attr<FieldType>::res value() const
        {
            if (this->_attr == 0)
                throw std::runtime_error(
                    "index " + zhttpd::Logger::toString(this->_index) +
                    " is out of range (" +
                    zhttpd::Logger::toString(T::__fields_len__) + ")");
            typedef typename cast_attr_type<FieldType>::res const* attr_type_t;
            attr_type_t attr = dynamic_cast<attr_type_t>(this->_attr);
            if (attr == 0)
                throw AttributeError("Wrong attribute cast for field '" + this->_attr->getName() + "'");
            return attr->getValue(*this->_item);
        }

        template<typename FieldType>
        AttributeIterator& operator =(FieldType val)
        {
            if (this->_attr == 0)
                throw std::runtime_error(
                    "index " + zhttpd::Logger::toString(this->_index) +
                    " is out of range (" +
                    zhttpd::Logger::toString(T::__fields_len__) + ")");
            typedef typename cast_attr_type<FieldType>::res const* attr_type_t;
            attr_type_t attr = dynamic_cast<attr_type_t>(this->_attr);
            if (attr == 0)
                throw AttributeError("Wrong attribute cast for field '" + this->_attr->getName() + "'");
            attr->setValue(*this->_item, val);
            return *this;
        }

        AttributeIterator& operator =(const char* str)//(typename pc_attr<FieldType>::res val)
        {
            if (this->_attr == 0)
                throw std::runtime_error(
                    "index " + zhttpd::Logger::toString(this->_index) +
                    " is out of range (" +
                    zhttpd::Logger::toString(T::__fields_len__) + ")");
            typedef typename cast_attr_type<std::string>::res const* attr_type_t;
            attr_type_t attr = dynamic_cast<attr_type_t>(this->_attr);
            if (attr == 0)
                throw AttributeError("Wrong attribute cast for field '" + this->_attr->getName() + "'");
            std::string val(str);
            attr->setValue(*this->_item, val);
            return *this;
        }
        AttributeIterator& operator =(int val)
        {
            if (this->_attr == 0)
                throw std::runtime_error(
                    "index " + zhttpd::Logger::toString(this->_index) +
                    " is out of range (" +
                    zhttpd::Logger::toString(T::__fields_len__) + ")");
            if (val > 0)
            {
                typedef typename cast_attr_type<unsigned int>::res const* attr_type_t;
                attr_type_t attr = dynamic_cast<attr_type_t>(this->_attr);
                if (attr != 0)
                {
                    attr->setValue(*this->_item, (unsigned int) val);
                    return *this;
                }
            }
            typedef typename cast_attr_type<int>::res const* attr_type_t;
            attr_type_t attr = dynamic_cast<attr_type_t>(this->_attr);
            if (attr == 0)
                throw AttributeError("Wrong attribute cast for field '" + this->_attr->getName() + "'");
            attr->setValue(*this->_item, val);
            return *this;
        }

    };
}

#endif /* !__ATTRIBUTEITERATOR_HPP__ */

