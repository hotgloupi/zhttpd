
#ifndef __TYPES_HPP__
# define __TYPES_HPP__

# include <cstddef>
# include <sys/types.h>

# ifdef _WIN32
#ifndef _WINDOWS_
#  include <Winsock2.h>
#  include <BaseTsd.h>
#endif
/**
 * Macro utilisée pour exporter un symbole dans les
 * librairies partagées.
 */
#  define ZHTTPD_EXPORT        __declspec(dllexport)
# else // UNIX
#  include <stdint.h>
/**
 * Macro utilisée pour exporter un symbole dans les
 * librairies partagées.
 */
#  define ZHTTPD_EXPORT
# endif // !UNIX


namespace zhttpd
{
    namespace api
    {
        typedef std::size_t     size_t;
        typedef std::ptrdiff_t  ssize_t;

# ifdef _WIN32
        typedef SOCKET          socket_t;
        typedef INT8            int8_t;
        typedef UINT8           uint8_t;
        typedef INT16           int16_t;
        typedef UINT16          uint16_t;
        typedef INT32           int32_t;
        typedef UINT32          uint32_t;
        typedef INT64           int64_t;
        typedef UINT64          uint64_t;
# else // UNIX
        typedef int             socket_t;
        typedef ::int8_t        int8_t;
        typedef ::uint8_t       uint8_t;
        typedef ::int16_t       int16_t;
        typedef ::uint16_t      uint16_t;
        typedef ::int32_t       int32_t;
        typedef ::uint32_t      uint32_t;
        typedef ::int64_t       int64_t;
        typedef ::uint64_t      uint64_t;
# endif // !UNIX
    }
}

#endif /* !__TYPES_HPP__ */

