
#include <cassert>

#include "db/DatabaseError.hpp"
#include "Statement.hpp"
#include "Cursor.hpp"

using namespace sqlite;

Cursor::Cursor(::sqlite3* db) :
    _db(db), _stmt(0), _status(db::status::UNDEFINED), _row(0), _last_rowid(0), _pending(false)
{
    assert(db != 0 && "Database pointer is NULL");
}

Cursor::Cursor(Cursor const& curs) :
    _db(curs._db), _stmt(0), _status(curs._status), _row(0), _last_rowid(0), _pending(false)
{
}

Cursor& Cursor::operator =(Cursor const& curs)
{
    if (this != &curs)
    {
        this->_db = curs._db;
        this->_stmt = curs._stmt;
        this->_status = curs._status;
        delete this->_row;
        this->_row = 0;
        this->_last_rowid = 0;
        this->_pending = false;
    }
    return *this;
}

Cursor::~Cursor()
{
    if (this->_pending)
    {
        this->_status = this->_stmt->step();
        this->_pending = false;
    }
    delete this->_row;
    this->_row = 0;
    delete this->_stmt;
    this->_stmt = 0;
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
    {
        if (this->_pending)
        {
            this->_stmt->step();
            this->_pending = false;
        }
        delete this->_stmt;
        this->_stmt = 0;
        delete this->_row;
        this->_row = 0;
        this->_last_rowid = 0;
    }
    this->_stmt = dynamic_cast<Statement*>(this->prepare(req));
    assert(this->_stmt != 0 && "Are you programming with your feet ??");
    this->_pending = true;
    return *this->_stmt;
}

zhttpd::api::uint64_t Cursor::lastrowid()
{
    if (this->_pending)
    {
        this->_status = this->_stmt->step();
        this->_last_rowid = ::sqlite3_last_insert_rowid(this->_db);
        this->_pending = false;
    }
    return this->_last_rowid;
}

db::IRow& Cursor::fetchone()
{
    if (this->_stmt == 0)
        throw db::DatabaseError("Cursor cannot fetch without any call to execute()");
    if (!this->hasData())
        throw db::DatabaseError("No more data is available (use hasData() before calling fetch())");
    this->_status = db::status::UNDEFINED;
    if (this->_row == 0)
        this->_row = new Row(this->_db, this->_stmt->getSqlite3Stmt());
    return *this->_row;
}

bool Cursor::hasData()
{
    if (this->_status == db::status::UNDEFINED)
        this->_status = this->_stmt->step();
    if (this->_pending)
    {
        this->_pending = false;
        this->_last_rowid = ::sqlite3_last_insert_rowid(this->_db);
    }
    switch (this->_status)
    {
    case db::status::READY:
        return true;
    case db::status::DONE:
        break;
    case db::status::BUSY:
        throw db::DatabaseError("Database is busy");
    default:
        throw db::DatabaseError(
            "Unknown status code '" +
            zhttpd::Logger::toString<int>(this->_status) +
            "'"
        );
    }
    delete this->_stmt;
    this->_stmt = 0;
    return false;
}

db::RowIterator Cursor::fetchall()
{
    return db::RowIterator();
}
