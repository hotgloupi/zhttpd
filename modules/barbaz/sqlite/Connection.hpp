/**
 * @file Connection.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__

# include <list>
# include <cassert>
# include <sqlite3.h>

# include "db/IConnection.hpp"

namespace sqlite
{
    class Connection : public db::IConnection
    {
    private:
        ::sqlite3* _db;
        std::list<db::ICursor*> _cursors;

    public:
        Connection(const char* filename);
        Connection(Connection const& conn);
        Connection& operator =(Connection const& conn);
        virtual ~Connection();
        virtual void close();
        virtual db::ICursor& cursor();
        virtual void commit();
    };

    static inline db::IConnection* open(const char* filename)
    {
        return new Connection(filename);
    }

    static inline void close(db::IConnection* conn)
    {
        assert(dynamic_cast<Connection*>(conn) != 0 && "Wrong connection type given !");
        delete conn;
    }
}

#endif /* !__CONNECTION_HPP__ */

