
#ifndef __SAFEALLOCALTOR_HPP__
# define __SAFEALLOCALTOR_HPP__

# include <set>
# include <cstring>

# include "thread/Mutex.hpp"
# include "utils/Logger.hpp"
# include "utils/macros.hpp"

namespace ZHTTPD
{
    template<typename Class>
    class SafeAllocator
    {
        typedef typename std::set<Class*> objects_t;

    private:
        objects_t           _objects;
        Mutex               _mutex;

    public:

        virtual ~SafeAllocator()
        {
            ZHTTPD_LOCK(this->_mutex);
            if (this->_objects.size() > 0)
            {
                LOG_WARN("Not all objects have been released : " +
                         Logger::toString(this->_objects.size()) +
                         " left");
                typename objects_t::iterator it = this->_objects.begin();
                typename objects_t::iterator end = this->_objects.end();
                for (; it != end; ++it)
                    delete *it;
                this->_objects.clear();
            }
            ZHTTPD_UNLOCK(this->_mutex);
            LOG_INFO("All buffer cleaned");
        }

        Class* allocate()
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = new Class();
            this->_objects.insert(res);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        template<typename T1>
        Class* allocate(T1& p1)
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = new Class(p1);
            this->_objects.insert(res);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        template<typename T1, typename T2>
        Class* allocate(T1& p1, T2& p2)
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = new Class(p1, p2);
            this->_objects.insert(res);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        template<typename T1, typename T2, typename T3>
        Class* allocate(T1& p1, T2& p2, T3& p3)
        {
            ZHTTPD_LOCK(this->_mutex);
            Class* res = new Class(p1, p2, p3);
            this->_objects.insert(res);
            ZHTTPD_UNLOCK(this->_mutex);
            return res;
        }

        void release(Class* obj)
        {
            ZHTTPD_LOCK(this->_mutex);
            if (this->_objects.find(obj) != this->_objects.end())
            {
                this->_objects.erase(obj);
                delete obj;
            }
            else
            {
                LOG_WARN("Wrong object given, already released or allocated by others");
            }
            ZHTTPD_UNLOCK(this->_mutex);
        }
    };
}

#endif /* !__SAFEALLOCALTOR_HPP__ */

