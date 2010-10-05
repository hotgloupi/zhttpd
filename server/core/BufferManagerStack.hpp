
#ifndef __BUFFERMANAGERSTACK_HPP__
# define __BUFFERMANAGERSTACK_HPP__

# include <list>

# include "utils/Singleton.hpp"
# include "utils/macros.hpp"
# include "thread/Mutex.hpp"
# include "api/types.hpp"

# include "BufferManager.hpp"

namespace ZHTTPD
{
    class BufferManagerStack : public Singleton<BufferManagerStack>
    {
        friend class Singleton<BufferManagerStack>;

    private:
        std::list<API::IBufferManager*>   _managers;
        Mutex                       _mutex;
        API::size_t                 _size;
        API::size_t                 _max;

    protected:
        BufferManagerStack() : _managers(), _mutex(), _size(0), _max(0)
        {
        }

        ~BufferManagerStack()
        {
            ZHTTPD_LOCK(this->_mutex);
            LOG_INFO("Max stored buffer managers number is " + Logger::toString(this->_max));
            LOG_INFO("Actual stored buffer managers number is " + Logger::toString(this->_max));
            std::list<API::IBufferManager*>::iterator it = this->_managers.begin();
            std::list<API::IBufferManager*>::iterator end = this->_managers.end();
            for (; it != end; ++it)
            {
                delete *it;
            }
            ZHTTPD_UNLOCK(this->_mutex);
        }
    public:
        inline API::IBufferManager* pop()
        {
            ZHTTPD_LOCK(this->_mutex);
            if (this->_size == 0) // TODO atomic fetch
            {
                ZHTTPD_UNLOCK(this->_mutex);
                return new BufferManager();
            }
            else
            {
                --this->_size;
                API::IBufferManager* res = this->_managers.back();
                this->_managers.pop_back();
                ZHTTPD_UNLOCK(this->_mutex);
                return res;
            }
        }

        inline void push(API::IBufferManager* manager)
        {
#ifdef ZHTTPD_DEBUG
            assert(dynamic_cast<BufferManager*>(manager) != 0);
#endif
            ZHTTPD_LOCK(this->_mutex);
            ++this->_size;
            if (this->_size > this->_max)
                this->_max = this->_size;
            this->_managers.push_back(manager);
            ZHTTPD_UNLOCK(this->_mutex);
        }
    };
}

#endif /* !__BUFFERMANAGERSTACK_HPP__ */

