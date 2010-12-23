
#include <stdlib.h>
#include <cassert>

#include "server/utils/Logger.hpp"
#include "db/DatabaseError.hpp"
#include "Value.hpp"
#include "Row.hpp"

using namespace sqlite;

Row::Row(::sqlite3* db, ::sqlite3_stmt* stmt) :
    _db(db), _stmt(stmt), _values(0), _column_count(0)
{
    assert(db != 0 && "Database pointer is NULL");
    assert(stmt != 0 && "Statement pointer is NULL");
    int res = ::sqlite3_column_count(stmt);
    if (res < 0)
        throw db::DatabaseError("Unexpected column count");
    this->_column_count = res;
    if (this->_column_count != 0)
    {
        this->_values = (db::IValue**) ::calloc(this->_column_count, sizeof(Value*));
        if (this->_values == 0)
            throw std::runtime_error("Bad alloc !");
    }
}

db::IValue& Row::operator[](unsigned int column)
{
    if (column >= this->_column_count)
        throw db::DatabaseError(
            "Out of range: " +
            zhttpd::Logger::toString(column) + " >= " +
            zhttpd::Logger::toString(this->_column_count)
        );
    if (this->_values[column] == 0)
    {
        this->_values[column] = new Value(this->_db, this->_stmt, column);
    }
    return *this->_values[column];
}

Row::~Row()
{
    for (unsigned int i = 0; i < this->_column_count; ++i)
    {
        delete this->_values[i];
        this->_values[i] = 0;
    }
    ::free(this->_values);
    this->_values = 0;
}
