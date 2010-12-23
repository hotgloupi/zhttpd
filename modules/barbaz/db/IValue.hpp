/**
 * @file IValue.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IVALUE_HPP__
# define __IVALUE_HPP__

# include <string>
# include "api/types.hpp"

namespace db
{
    class IValue
    {
    public:
        virtual int getInt() = 0;
        virtual zhttpd::api::int64_t getInt64() = 0;
        virtual zhttpd::api::uint64_t getUint64() = 0;
        virtual float getFloat() = 0;
        virtual double getDouble() = 0;
        virtual std::string getString() = 0;
        virtual bool getBool() = 0;
        virtual ~IValue() {}
    };
}

#endif /* !__IVALUE_HPP__ */

