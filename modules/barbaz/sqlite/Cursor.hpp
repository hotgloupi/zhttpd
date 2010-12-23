/**
 * @file Cursor.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __CURSOR_HPP__
# define __CURSOR_HPP__

# include <sqlite3.h>

# include "db/status.hpp"
# include "db/ICursor.hpp"
# include "Statement.hpp"
# include "Row.hpp"

namespace sqlite
{
    class Cursor : public db::ICursor
    {
    private:
        ::sqlite3* _db;
        Statement* _stmt;
        db::status::Type _status;
        db::IRow* _row;
        unsigned int _count;

    public:
        Cursor(::sqlite3* db);
        Cursor(Cursor const& curs);
        Cursor& operator =(Cursor const& curs);
        virtual db::IStatement& execute(char const* req);
        virtual db::IStatement* prepare(char const* req);
        virtual db::IRow& fetchone();
        virtual db::RowIterator fetchall();
        virtual bool hasData();
    };
}

#endif /* !__CURSOR_HPP__ */

