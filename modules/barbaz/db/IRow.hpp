
#ifndef __IROW_HPP__
# define __IROW_HPP__

# include "IItem.hpp"
# include "IValue.hpp"

namespace db
{
    class IRow
    {
    public:
        virtual IValue& operator[](unsigned int column) = 0;
        virtual void fillItem(IItem& item) = 0;
        virtual ~IRow() {}
    };
}

#endif /* !__IROW_HPP__ */

