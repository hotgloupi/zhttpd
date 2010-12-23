/**
 * @file IConnection.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * Represent a connection to a database
 */

#ifndef __ICONNECTION_HPP__
# define __ICONNECTION_HPP__

# include "ICursor.hpp"

namespace db
{
    class IConnection
    {
    public:
        virtual void close() = 0;
        virtual ICursor* cursor() = 0;
        virtual void commit() = 0;
        virtual ~IConnection() {}
    };
}

#endif /* !__ICONNECTION_HPP__ */

