
#include <stdlib.h>
#include <cassert>

#include "server/utils/Logger.hpp"
#include "db/DatabaseError.hpp"
#include "db/IVisitor.hpp"
#include "db/Attributes.hpp"
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

struct RowVisitor : public db::IVisitor
{
    unsigned int index;
    Row& row;

    RowVisitor(Row& row) : index(0), row(row) {}
    void visitInt(db::AttributeInt const& attr, db::IItem& i)       { attr.setValue(i, row[this->index++].getInt()); }
    void visitInt64(db::AttributeInt64 const& attr, db::IItem& i)   { attr.setValue(i, row[this->index++].getInt64()); }
    void visitUint64(db::AttributeUint64 const& attr, db::IItem& i) { attr.setValue(i, row[this->index++].getInt64()); }
    void visitDouble(db::AttributeDouble const& attr, db::IItem& i) { attr.setValue(i, row[this->index++].getDouble()); }
    void visitFloat(db::AttributeFloat const& attr, db::IItem& i)   { attr.setValue(i, row[this->index++].getDouble()); }
    void visitBool(db::AttributeBool const& attr, db::IItem& i)     { attr.setValue(i, row[this->index++].getInt()); }
    void visitString(db::AttributeString const& attr, db::IItem& i)
    {
        std::string s = row[this->index++].getString();
        attr.setValue(i, s);
    }
};

void Row::fillItem(db::IItem& item)
{
    RowVisitor v(*this);
    item.visitAll(v);
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
