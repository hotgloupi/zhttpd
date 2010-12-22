/**
 * @file Attributes.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ATTRIBUTES_HPP__
# define __ATTRIBUTES_HPP__

# include <string>
# include <cassert>
# include <stdint.h>

# include "IVisitor.hpp"
# include "Attribute.hpp"
//# include "api/type.hpp"

namespace db
{

# define ATTRIBUTE_CLS(suffix, type)                                          \
    class Attribute##suffix : public Attribute<type>                          \
    {                                                                         \
    public:                                                                   \
        Attribute##suffix(char const* name, getter_t get, setter_t set) :     \
            Attribute<type>(name, get, set) {}                                \
        virtual void visit(IVisitor& v, IItem& i) const                       \
        {                                                                     \
            v.visit##suffix(*this, i);                                        \
        }                                                                     \
        virtual void visit(IVisitor& v, IItem const& i) const                 \
        {                                                                     \
            v.visit##suffix(*this, i);                                        \
        }                                                                     \
    }

    ATTRIBUTE_CLS(Int, int);
    ATTRIBUTE_CLS(Int64, int64_t);
    ATTRIBUTE_CLS(Uint64, uint64_t);
    ATTRIBUTE_CLS(Float, float);
    ATTRIBUTE_CLS(Double, double);
    ATTRIBUTE_CLS(Bool, bool);
    ATTRIBUTE_CLS(String, std::string);

# undef ATTRIBUTE_CLS
}

#endif /* !__ATTRIBUTES_HPP__ */

