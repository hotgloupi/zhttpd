/**
 * @file DatabaseError.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __DATABASEERROR_HPP__
# define __DATABASEERROR_HPP__

# include <string>
# include <stdexcept>

namespace db
{
    class DatabaseError : public std::exception
    {
    private:
        std::string _msg;

    public:
        DatabaseError(std::string const& msg) : _msg(msg) {}
        virtual const char* what() const throw() { return this->_msg.c_str(); }
        virtual ~DatabaseError() throw() {}
    };
}

#endif /* !__DATABASEERROR_HPP__ */

