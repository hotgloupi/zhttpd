
#ifndef __SIMPLEALLOCATOR_HPP__
# define __SIMPLEALLOCATOR_HPP__

# include "api/types.hpp"

namespace ZHTTPD
{
    class SimpleAllocator
    {
    public:
        char* allocate(API::size_t size);
        char* resize(char* data, API::size_t old_size, API::size_t new_size);
        void release(char* data);
    };
}

#endif /* !__SIMPLEALLOCATOR_HPP__ */

