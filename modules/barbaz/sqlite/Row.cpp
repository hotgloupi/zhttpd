
#include <stdlib.h>
#include <cassert>
#include <map>

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

struct FillItemVisitor : public db::IVisitor
{
    typedef std::map<std::string, Value*> map_t;
    map_t& values;

    FillItemVisitor(map_t& values) : values(values)
    {
    }

    void visitInt(db::AttributeInt const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
            attr.setValue(i, it->second->getInt());
    }

    void visitInt64(db::AttributeInt64 const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
            attr.setValue(i, it->second->getInt64());
    }

    void visitUint64(db::AttributeUint64 const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
            attr.setValue(i, it->second->getInt64());
    }

    void visitDouble(db::AttributeDouble const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
            attr.setValue(i, it->second->getDouble());
    }

    void visitFloat(db::AttributeFloat const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
            attr.setValue(i, it->second->getFloat());
    }

    void visitBool(db::AttributeBool const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
            attr.setValue(i, it->second->getBool());
    }

    void visitString(db::AttributeString const& attr, db::IItem& i)
    {
        map_t::iterator it = this->values.find(attr.getName());
        if (it != this->values.end())
        {
            std::string s = it->second->getString();
            attr.setValue(i, s);
        }
    }
};

void Row::fillItem(db::IItem& item)
{
    std::map<std::string, Value*> values;
    for (unsigned int i = 0; i < this->_column_count; ++i)
    {
        const char* name = ::sqlite3_column_name(this->_stmt, i);
        if (name != 0)
        {
            values[name] = dynamic_cast<Value*>(&(*this)[i]);
            assert(values[name] != 0 && "Not valid value ?! Huu ?!");
        }
        else
            throw db::DatabaseError("Column " + zhttpd::Logger::toString(i) + " with no name");
    }
    FillItemVisitor v(values);
    item.visitAll(v);
}

Row::~Row()
{
    LOG_INFO("Delete ROW");
    for (unsigned int i = 0; i < this->_column_count; ++i)
    {
        delete this->_values[i];
        this->_values[i] = 0;
    }
    ::free(this->_values);
    this->_values = 0;
}
