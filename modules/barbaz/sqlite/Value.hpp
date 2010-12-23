/**
 * @file Value.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __VALUE_HPP__
# define __VALUE_HPP__

# include <stdexcept>
# include <sqlite3.h>

# include "db/IValue.hpp"

namespace sqlite
{
    class Value : public db::IValue
    {
    private:
        ::sqlite3* _db;
        ::sqlite3_stmt* _stmt;
        unsigned int _index;
        void* _data;

    public:
        Value(::sqlite3* db, ::sqlite3_stmt* stmt, unsigned int index) :
            _db(db), _stmt(stmt), _index(index), _data(0)
        {
        }

        virtual int getInt()
        {
            int res = ::sqlite3_column_int(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return res;
        }

        virtual zhttpd::api::int64_t getInt64()
        {
            zhttpd::api::int64_t res = ::sqlite3_column_int64(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return res;
        }


        virtual zhttpd::api::uint64_t getUint64()
        {
            zhttpd::api::uint64_t res = ::sqlite3_column_int64(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return res;
        }

        virtual float getFloat()
        {
            float res = ::sqlite3_column_double(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return res;
        }

        virtual double getDouble()
        {
            double res = ::sqlite3_column_double(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return res;
        }

        virtual std::string getString()
        {
            const unsigned char* res = ::sqlite3_column_text(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return std::string((const char*) res);
        }

        virtual bool getBool()
        {
            int res = ::sqlite3_column_int(this->_stmt, this->_index);
            if (res == 0 && ::sqlite3_errcode(this->_db) == SQLITE_NOMEM)
                throw std::runtime_error("Out of memory");
            return res;
        }

    private:
        Value(Value const& value);
        Value& operator =(Value const& value);
    };
}

#endif /* !__VALUE_HPP__ */

