/**
 * @file IRow.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __IROW_HPP__
# define __IROW_HPP__

# include "IValue.hpp"

namespace db
{
    class IRow
    {
    public:
        virtual IValue& operator[](unsigned int column) = 0;
        virtual ~IRow() {}
    };
}

#endif /* !__IROW_HPP__ */

