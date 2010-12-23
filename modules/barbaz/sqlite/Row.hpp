/**
 * @file Row.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ROW_HPP__
# define __ROW_HPP__

# include <sqlite3.h>

# include "db/IRow.hpp"
# include "Value.hpp"

namespace sqlite
{
    class Row : public db::IRow
    {
    private:
        sqlite3* _db;
        sqlite3_stmt* _stmt;
        db::IValue** _values;
        unsigned int _column_count;

    public:
        Row(::sqlite3* db, ::sqlite3_stmt* stmt);
        virtual db::IValue& operator[](unsigned int column);
        virtual ~Row();
    };
}

#endif /* !__ROW_HPP__ */

