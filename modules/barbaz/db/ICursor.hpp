/**
 * @file ICursor.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * Represent a cursor to a database connection
 */

#ifndef __ICURSOR_HPP__
# define __ICURSOR_HPP__

# include "IRow.hpp"
# include "RowIterator.hpp"
# include "IStatement.hpp"

namespace db
{
    class ICursor
    {
    public:
        virtual IStatement& execute(char const* req) = 0;
        virtual IStatement* prepare(char const* req) = 0;
        virtual IRow& fetchone() = 0;
        virtual RowIterator fetchall() = 0;
        virtual bool hasData() = 0;
        virtual ~ICursor() {}
    };
}

#endif /* !__ICURSOR_HPP__ */

