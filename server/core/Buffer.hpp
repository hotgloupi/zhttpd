
#ifndef __BUFFER_HPP__
# define __BUFFER_HPP__

# include <cstring>     // memcpy

# include "api/IBuffer.hpp"
# include "utils/NonCopyable.hpp"
# include "utils/StatsManager.hpp"

namespace zhttpd
{
    template<typename Allocator>
    class Buffer : public api::IBuffer, public NonCopyable
    {
    private:
        Allocator* _allocator;
        char* _data;
        api::size_t _size;
        api::size_t _offset;

    public:
        Buffer(Allocator& allocator, char const* data, api::size_t size) :
            _allocator(&allocator),
            _data(0),
            _size(size),
            _offset(0)
        {
            this->_data = this->_allocator->allocate(size);
            ::memcpy(this->_data, data, size);
            StatsManager::getInstance()->addBuffer();
        }

        Buffer(Allocator& allocator, api::size_t size) :
            _allocator(&allocator),
            _data(0),
            _size(size),
            _offset(0)
        {
            this->_data = this->_allocator->allocate(size);
            StatsManager::getInstance()->addBuffer();
        }

        virtual ~Buffer()
        {
            this->_allocator->release(this->_data, this->_size);
            StatsManager::getInstance()->removeBuffer();
        }

        char* getRawData()
        {
            return this->_data + this->_offset;
        }

        api::size_t getSize() const
        {
            return this->_size;
        }

        void setSize(api::size_t size)
        {
            this->_data = this->_allocator->resize(this->_data, this->_offset + this->_size, this->_offset + size);
            this->_size = size;
        }

        void setSize(api::size_t start, api::size_t size)
        {
            this->_offset += start;
            this->setSize(start + size);
        }
    };
}

#endif /* !__BUFFER_HPP__ */

