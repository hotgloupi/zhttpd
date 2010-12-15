
#ifndef __IBUFFER_HPP__
# define __IBUFFER_HPP__

# include "types.hpp"

namespace zhttpd
{
    namespace api
    {
        class IBuffer
        {
        public:
            /**
             * Get buffer raw data. The returned pointer is never null.
             */
            virtual char* getRawData() = 0;

            /**
             * Returns the buffer size (always > 0).
             */
            virtual api::size_t getSize() const = 0;

            /**
             * Resize the buffer to the specified size.
             * It acts exactly like realloc, so data are preserved.
             * @param size Size wanted.
             */
            virtual void setSize(api::size_t size) = 0;

            /**
             * Resize the buffer to the specified size.
             * @param start Start offset (data before are lost)
             * @param size Total new size
             * @example
             *  b = buffer_manager.allocate("abcdef");
             *  b.setSize(2, 4); // b contains "cdef"
             */
            virtual void setSize(api::size_t start, api::size_t size) = 0;

            virtual ~IBuffer() {}
        };
    }
}

#endif /* !__IBUFFER_HPP__ */

