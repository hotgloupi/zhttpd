/**
 * @file AttributeError.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ATTRIBUTEERROR_HPP__
# define __ATTRIBUTEERROR_HPP__

# include <string>
# include <stdexcept>

namespace db
{
    class AttributeError : public std::exception
    {
    private:
        std::string _msg;

    public:
        AttributeError(std::string const& msg) : _msg(msg) {}
        virtual const char* what() const throw() { return this->_msg.c_str(); }
        virtual ~AttributeError() throw() {}
    };
}

#endif /* !__ATTRIBUTEERROR_HPP__ */

