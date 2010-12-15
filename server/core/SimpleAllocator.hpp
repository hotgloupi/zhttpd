
#ifndef __SIMPLEALLOCATOR_HPP__
# define __SIMPLEALLOCATOR_HPP__

# include "api/types.hpp"

namespace zhttpd
{
    class SimpleAllocator
    {
    public:
        char* allocate(api::size_t size);
        char* resize(char* data, api::size_t old_size, api::size_t new_size);
        void release(char* data);
    };
}

#endif /* !__SIMPLEALLOCATOR_HPP__ */

