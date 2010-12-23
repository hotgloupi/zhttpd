/**
 * @file IStatement.hpp
 * @author <@> @NAME@
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __ISTATEMENT_HPP__
# define __ISTATEMENT_HPP__

#include <string>

#include "api/types.hpp"

namespace db
{
    class IStatement
    {
    public:
        virtual IStatement& bind(int val) = 0;
        virtual IStatement& bind(unsigned int val) = 0;
        virtual IStatement& bind(zhttpd::api::int64_t val) = 0;
        virtual IStatement& bind(zhttpd::api::uint64_t val) = 0;
        virtual IStatement& bind(float val) = 0;
        virtual IStatement& bind(double val) = 0;
        virtual IStatement& bind(bool val) = 0;
        virtual IStatement& bind(std::string const& s) = 0;
        virtual IStatement& bind(const char* s) = 0;
        virtual ~IStatement() {}
    };
}

#endif /* !__ISTATEMENT_HPP__ */

