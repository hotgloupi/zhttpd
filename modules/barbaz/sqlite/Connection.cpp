
#include <sqlite3.h>

#include "db/DatabaseError.hpp"
#include "Connection.hpp"
#include "Cursor.hpp"

using namespace sqlite;

Connection::Connection(const char* filename) : _db(0)
{
    int res = ::sqlite3_open(filename, &this->_db);
    if (res != 0)
    {
        std::string msg(::sqlite3_errmsg(this->_db));
        ::sqlite3_close(this->_db);
        this->_db = 0;
        throw db::DatabaseError(
            "Cannot open '" + std::string(filename) + "': " + msg
        );
    }
}

Connection::Connection(Connection const& conn) : _db(conn._db) {}

Connection& Connection::operator =(Connection const& conn)
{
    if (this != &conn)
    {
        this->_db = conn._db;
    }
    return *this;
}

void Connection::close()
{
    if (this->_db != 0)
        ::sqlite3_close(this->_db);
}

db::ICursor* Connection::cursor()
{
    return new Cursor(this->_db);
}

void Connection::commit()
{

}
