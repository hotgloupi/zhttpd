
#include <cassert>

#include "db/DatabaseError.hpp"
#include "Statement.hpp"
#include "Cursor.hpp"

using namespace sqlite;

Cursor::Cursor(::sqlite3* db) :
    _db(db), _stmt(0), _status(db::status::UNDEFINED)
{
    assert(db != 0 && "Database pointer is NULL");
}

Cursor::Cursor(Cursor const& curs) :
    _db(curs._db), _stmt(0), _status(curs._status)
{
}

Cursor& Cursor::operator =(Cursor const& curs)
{
    if (this != &curs)
    {
        this->_db = curs._db;
        this->_stmt = curs._stmt;
        this->_status = curs._status;
    }
    return *this;
}

db::IStatement* Cursor::prepare(char const* req)
{
    assert(req != 0 && "Given request is null !");
    ::sqlite3_stmt* stmt;
    int res = ::sqlite3_prepare_v2(
        this->_db,     // Database handle
        req,           // SQL statement, UTF-8 encoded
        -1,            // Maximum length of zSql in bytes.
        &stmt,         // OUT: Statement handle
        0              // OUT: Pointer to unused portion of zSql
    );
    if (res != SQLITE_OK)
    {
        std::string msg = "Cannot prepare the request '" + std::string(req) + "': " + ::sqlite3_errmsg(this->_db);
        (void)::sqlite3_finalize(stmt);
        throw db::DatabaseError(msg);
    }
    return new Statement(this->_db, stmt);
}

db::IStatement& Cursor::execute(char const* req)
{
    if (this->_stmt != 0)
        throw db::DatabaseError("Cursor is reused without closing it first");
    this->_stmt = dynamic_cast<Statement*>(this->prepare(req));
    assert(this->_stmt != 0 && "Are you programming with your feet ??");
    this->_row = new Row(this->_db, this->_stmt->getSqlite3Stmt());
    return *this->_stmt;
}

db::IRow& Cursor::fetchone()
{
    if (this->_stmt == 0)
        throw db::DatabaseError("Cursor cannot fetch without any call to execute()");
    if (this->_status == db::status::UNDEFINED)
        this->_status = this->_stmt->step();
    switch (this->_status)
    {
    case db::status::UNDEFINED:
        throw db::DatabaseError("fetchone() called while database status is undefined");
    case db::status::READY:
        this->_status = db::status::UNDEFINED;
        return *this->_row;
    case db::status::DONE:
        throw db::DatabaseError("No more data is available (use hasData() before calling fetch())");
    case db::status::BUSY:
        throw db::DatabaseError("Database is busy");
    default:
        throw db::DatabaseError(
            "Unknown status code '" +
            zhttpd::Logger::toString<int>(this->_status) +
            "'"
        );
    }

}

bool Cursor::hasData()
{
    if (this->_status == db::status::UNDEFINED)
        this->_status = this->_stmt->step();

    switch (this->_status)
    {
    case db::status::READY:
        return true;
    case db::status::DONE:
        return false;
    case db::status::BUSY:
        throw db::DatabaseError("Database is busy");
    default:
        throw db::DatabaseError(
            "Unknown status code '" +
            zhttpd::Logger::toString<int>(this->_status) +
            "'"
        );
    }
}

db::RowIterator Cursor::fetchall()
{
    return db::RowIterator();
}
