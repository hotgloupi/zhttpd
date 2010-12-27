/**
 * @file Statement.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __STATEMENT_HPP__
# define __STATEMENT_HPP__

# include <sqlite3.h>

# include "server/utils/Logger.hpp"
# include "db/DatabaseError.hpp"
# include "db/IStatement.hpp"
# include "db/status.hpp"

namespace sqlite
{
    class Statement : public db::IStatement
    {
    private:
        ::sqlite3* _db;
        ::sqlite3_stmt* _stmt;
        int _index;

    public:
        Statement(::sqlite3* db, ::sqlite3_stmt* stmt) : _db(db), _stmt(stmt), _index(1)
        {
            assert(this->_stmt != 0 && "Statement is NULL");
        }

        virtual ~Statement()
        {
            LOG_INFO("DELETE STATEMENT");
            ::sqlite3_finalize(this->_stmt);
        }

        virtual IStatement& bind(int val)
        {
            int res = ::sqlite3_bind_int(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(unsigned int val)
        {
            int res = ::sqlite3_bind_int(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(zhttpd::api::int64_t val)
        {
            int res = ::sqlite3_bind_int64(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(zhttpd::api::uint64_t val)
        {
            int res = ::sqlite3_bind_int64(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(float val)
        {
            int res = ::sqlite3_bind_double(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(double val)
        {
            int res = ::sqlite3_bind_double(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(bool val)
        {
            int res = ::sqlite3_bind_int(this->_stmt, this->_index, val);
            if (res != SQLITE_OK)
                this->_raiseError(val);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(std::string const& s)
        {
            int res = ::sqlite3_bind_text(this->_stmt, this->_index, s.c_str(), -1, SQLITE_TRANSIENT);
            if (res != SQLITE_OK)
                this->_raiseError(s);
            this->_index += 1;
            return *this;
        }

        virtual IStatement& bind(const char* s)
        {
            int res = ::sqlite3_bind_text(this->_stmt, this->_index, s, -1, SQLITE_TRANSIENT);
            if (res != SQLITE_OK)
                this->_raiseError(s);
            this->_index += 1;
            return *this;
        }

        db::status::Type step()
        {
            int res = ::sqlite3_step(this->_stmt);
            switch (res)
            {
            case SQLITE_DONE:
                return db::status::DONE;
            case SQLITE_BUSY:
                return db::status::BUSY;
            case SQLITE_ROW:
                return db::status::READY;
            default:
                throw db::DatabaseError(
                    "Cannot go forward: " +
                    std::string(::sqlite3_errmsg(this->_db))
                );
            }
        }

        ::sqlite3_stmt* getSqlite3Stmt() { return this->_stmt; }

    private:
        template<typename T>
        void _raiseError(T val)
        {
            throw db::DatabaseError(
                "Cannot bind value '" + zhttpd::Logger::toString(val) +
                "' to index " + zhttpd::Logger::toString(this->_index) +
                ": " + ::sqlite3_errmsg(this->_db)
            );
        }
    };
}

#endif /* !__STATEMENT_HPP__ */

