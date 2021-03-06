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
        zhttpd::api::uint64_t _last_rowid;
        bool _pending;

    public:
        Cursor(::sqlite3* db);
        Cursor(Cursor const& curs);
        Cursor& operator =(Cursor const& curs);
        virtual ~Cursor();
        virtual db::IStatement& execute(char const* req);
        virtual db::IStatement* prepare(char const* req);
        virtual db::IRow& fetchone();
        virtual zhttpd::api::uint64_t lastrowid();
        virtual db::RowIterator fetchall();
        virtual bool hasData();
    };
}

#endif /* !__CURSOR_HPP__ */

