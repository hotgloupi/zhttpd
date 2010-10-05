#ifndef __BUFFERALLOCATOR_HPP__
#define __BUFFERALLOCATOR_HPP__

#include <vector>
#ifdef ZHTTPD_DEBUG
    #include <map>
#endif

#include "api/types.hpp"

namespace ZHTTPD
{
    class BufferAllocator
    {
        public:
            BufferAllocator(API::size_t size = 10000);
            virtual ~BufferAllocator();
            char* allocate(API::size_t size);
            char* resize(char* data, API::size_t old_size, API::size_t new_size);
            void release(char* data, API::size_t size);
            void clean();
        private:
            enum
            {
                RESERVE_SIZE = 10000,
                BLOCK_SIZE = 4096
            };
            std::vector<char*> _free_blocks;
            std::vector<char*> _blocks;
#ifdef ZHTTPD_DEBUG
            std::map<char*, API::size_t> _debug;
#endif
    };
}

#endif

