
# include <cstring> // memcpy

# include "SimpleAllocator.hpp"

using namespace zhttpd;

char* SimpleAllocator::allocate(api::size_t size)
{
    return new char[size];
}

char* SimpleAllocator::resize(char* data, api::size_t old_size, api::size_t new_size)
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
