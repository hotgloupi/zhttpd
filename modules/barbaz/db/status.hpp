/**
 * @file status.hpp
 * @author <raphael.londeix@gmail.com> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __STATUS_HPP__
# define __STATUS_HPP__

namespace db
{
    namespace status
    {
        enum Type
        {
            UNDEFINED,
            READY,
            DONE,
            BUSY,
        };
    }
}

#endif /* !__STATUS_HPP__ */

