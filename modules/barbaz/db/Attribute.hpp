/**
 * @file Attribute.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ATTRIBUTE_HPP__
# define __ATTRIBUTE_HPP__

# include "IItem.hpp"
# include "IAttribute.hpp"
# include "AttributesMacros.hpp"

namespace db
{
    template<typename T>
    class Attribute : public IAttribute
    {
    public:
        typedef typename pc_const_attr<T>::res (IItem::*getter_t)() const;
        typedef void (IItem::*setter_t)(typename pc_attr<T>::res val);

    protected:
        std::string _name;
        getter_t _get_val;
        setter_t _set_val;

    public:
        Attribute(char const* name, getter_t get, setter_t set) :
            _name(name), _get_val(get), _set_val(set)
        {
        }

        virtual std::string const& getName() const { return this->_name; }

        virtual typename pc_const_attr<T>::res getValue(IItem const& item) const
        {
            assert(this->_get_val != 0 && "Attribute getter is NULL");
            return (item.*(this->_get_val))();
        }

        virtual void setValue(IItem& item, typename pc_attr<T>::res value) const
        {
            assert(this->_set_val != 0 && "Attribute setter is NULL");
            (item.*(this->_set_val))(value);
        }

    };
}

#endif /* !__ATTRIBUTE_HPP__ */

