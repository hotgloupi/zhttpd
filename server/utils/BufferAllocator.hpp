#ifndef __BUFFERALLOCATOR_HPP__
#define __BUFFERALLOCATOR_HPP__

#include <vector>
#ifdef ZHTTPD_DEBUG
    #include <map>
#endif

#include "api/types.hpp"

namespace zhttpd
{
    class BufferAllocator
    {
        public:
            BufferAllocator(api::size_t size = 10000);
            virtual ~BufferAllocator();
            char* allocate(api::size_t size);
            char* resize(char* data, api::size_t old_size, api::size_t new_size);
            void release(char* data, api::size_t size);
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
            std::map<char*, api::size_t> _debug;
#endif
    };
}

#endif

