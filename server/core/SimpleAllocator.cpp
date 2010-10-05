
# include <cstring> // memcpy

# include "SimpleAllocator.hpp"

using namespace ZHTTPD;

char* SimpleAllocator::allocate(API::size_t size)
{
    return new char[size];
}

char* SimpleAllocator::resize(char* data, API::size_t old_size, API::size_t new_size)
{
    if (new_size <= old_size)
    {
        return data;
    }
    else
    {
        char *new_data = new char[new_size];
        ::memcpy(new_data, data, old_size);
        this->release(data);
        return new_data;
    }
}

void SimpleAllocator::release(char* data)
{
    delete [] data;
}
