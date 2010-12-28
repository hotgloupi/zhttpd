/**
 * @file AttributesMacros.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ATTRIBUTESMACROS_HPP__
# define __ATTRIBUTESMACROS_HPP__

namespace db
{
    template<typename T> struct cast_attr {typedef T res;};
    template<> struct cast_attr<time_t> {typedef uint64_t res;};
    template<> struct cast_attr<unsigned int> {typedef uint64_t res;};


    template<typename T> struct pass_attr{typedef T& res;};
    template<> struct pass_attr<int>{typedef int res;};
    template<> struct pass_attr<unsigned int>{typedef unsigned int res;};
    template<> struct pass_attr<uint64_t>{typedef uint64_t res;};
    template<> struct pass_attr<int64_t>{typedef int64_t res;};
    template<> struct pass_attr<float>{typedef float res;};
    template<> struct pass_attr<double>{typedef double res;};
    template<> struct pass_attr<bool>{typedef bool res;};

    template<typename T> struct pc_attr
    {
        typedef typename pass_attr<typename cast_attr<T>::res>::res res;
    };

    template<typename T> struct pc_const_attr
    {
        typedef typename pc_attr<T>::res res;
    };
    template<> struct pc_const_attr<std::string>
    {
        typedef std::string const& res;
    };

    template<typename T> struct attr_type;
    template<> struct attr_type<int64_t> {typedef AttributeInt64 res;};
    template<> struct attr_type<int> {typedef AttributeInt res;};
    template<> struct attr_type<uint64_t> {typedef AttributeUint64 res;};
    template<> struct attr_type<float> {typedef AttributeFloat res;};
    template<> struct attr_type<double> {typedef AttributeDouble res;};
    template<> struct attr_type<bool> {typedef AttributeBool res;};
    template<> struct attr_type<std::string> {typedef AttributeString res;};

    template<typename T> struct cast_attr_type
    {
        typedef typename attr_type<typename cast_attr<T>::res>::res res;
    };

    template<typename T>
    struct ItemFieldsDeallocator
    {
        ~ItemFieldsDeallocator()
        {
            for (unsigned int i = 0; i < T::__fields_len__; ++i)
            {
                delete T::__fields__[i];
            }
        }
    };
}

# define DECLARE_ATTRIBUTE_CLASS(name)\
    public:\
        name();\
        static db::IAttribute const* const __fields__[];\
        static unsigned int const __fields_len__;\
        static char const* const __name__

# define DEFINE_ATTRIBUTE_CLASS(name, ...)\
db::IAttribute const* const name::__fields__[] = {__VA_ARGS__};\
unsigned int const name::__fields_len__ = sizeof(name::__fields__) / sizeof(*name::__fields__);\
char const* const name::__name__ = #name;\
name::name()\
{\
     for (unsigned int i = 0; i < name::__fields_len__; ++i)\
         name::__fields__[i]->setDefaultValue(*this);\
}\
static db::ItemFieldsDeallocator<name> _item_fiels_destructor_for_##name


# define ATTR_GETSET(field, type)\
    public:\
        virtual db::pc_const_attr<type>::res get_##field() const\
        {\
            return this->_##field;\
        }\
        virtual void set_##field(db::pc_attr<type>::res value)\
        {\
            this->_##field = value;\
        }\
    protected:\
        type _##field

# define NEW_FIELD(cls, name, type)\
    new db::cast_attr_type<type>::res(\
            #name,\
            static_cast<db::pc_const_attr<type>::res (IItem::*)() const>(&cls::get_##name),\
            static_cast<void (IItem::*)(db::pc_attr<type>::res)>(&cls::set_##name)\
    )


#endif /* !__ATTRIBUTESMACROS_HPP__ */

