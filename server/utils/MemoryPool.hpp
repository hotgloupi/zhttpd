
#ifndef __MEMORYPOOL_HPP__
# define __MEMORYPOOL_HPP__

# include <vector>
# include <algorithm>
# ifdef ZHTTPD_DEBUG
#  include <set>
# endif

# include "api/types.hpp"
# include "utils/Logger.hpp"
# include "utils/StatsManager.hpp"
# include "thread/Mutex.hpp"

namespace zhttpd
{
    template <typename C>
    class MemoryPool
    {
    private:
        std::vector<C*> _free_blocks;
        std::vector<C*> _blocks;
# ifdef ZHTTPD_DEBUG
        std::set<C*> _debug;
        Mutex _mutex;
#  define DEBUG_INSERT(block) this->_debug.insert(block)
# else
#  define DEBUG_INSERT(block) (static_cast<void>(0))
# endif

    public:
        MemoryPool(api::size_t size = 1000)
        {
            this->_free_blocks.reserve(size);
            this->_blocks.reserve(size);
        }

        ~MemoryPool()
        {
            this->clean();
            if (!this->_blocks.empty())
                LOG_WARN("There are blocks left (all buffers have not been released).");
        }

        C* allocate()
        {
            ZHTTPD_ASSERT_LOCK(this->_mutex);
            if (this->_free_blocks.empty())
            {
                StatsManager::getInstance()->addMemory(sizeof(C));
                C* n = new C();
                this->_blocks.push_back(n);
                DEBUG_INSERT(n);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return n;
            }
            C* ret = this->_free_blocks.back();
            this->_free_blocks.pop_back();
            DEBUG_INSERT(ret);
            ZHTTPD_ASSERT_UNLOCK(this->_mutex);
            return new (ret) C();
        }

        template <typename T1>
        C* allocate(T1& p1)
        {
            ZHTTPD_ASSERT_LOCK(this->_mutex);
            if (this->_free_blocks.empty())
            {
                StatsManager::getInstance()->addMemory(sizeof(C));
                C* n = new C(p1);
                this->_blocks.push_back(n);
                DEBUG_INSERT(n);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return n;
            }
            C* ret = this->_free_blocks.back();
            this->_free_blocks.pop_back();
            DEBUG_INSERT(ret);
            ZHTTPD_ASSERT_UNLOCK(this->_mutex);
            return new (ret) C(p1);
        }

        template <typename T1, typename T2>
        C* allocate(T1& p1, T2& p2)
        {
            ZHTTPD_ASSERT_LOCK(this->_mutex);
            if (this->_free_blocks.empty())
            {
                StatsManager::getInstance()->addMemory(sizeof(C));
                C* n = new C(p1, p2);
                this->_blocks.push_back(n);
                DEBUG_INSERT(n);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return n;
            }
            C* ret = this->_free_blocks.back();
            this->_free_blocks.pop_back();
            DEBUG_INSERT(ret);
            ZHTTPD_ASSERT_UNLOCK(this->_mutex);
            return new (ret) C(p1, p2);
        }

        template <typename T1, typename T2, typename T3>
        C* allocate(T1& p1, T2& p2, T3& p3)
        {
            ZHTTPD_ASSERT_LOCK(this->_mutex);
            if (this->_free_blocks.empty())
            {
                StatsManager::getInstance()->addMemory(sizeof(C));
                C* n = new C(p1, p2, p3);
                this->_blocks.push_back(n);
                DEBUG_INSERT(n);
                ZHTTPD_ASSERT_UNLOCK(this->_mutex);
                return n;
            }
            C* ret = this->_free_blocks.back();
            this->_free_blocks.pop_back();
            DEBUG_INSERT(ret);
            ZHTTPD_ASSERT_UNLOCK(this->_mutex);
            return new (ret) C(p1, p2, p3);
        }

        void release(C* obj)
        {
            ZHTTPD_ASSERT_LOCK(this->_mutex);
#ifdef ZHTTPD_DEBUG
            if (this->_debug.count(obj) == 0)
                LOG_ERROR("Releasing unknown object.");
            else
                this->_debug.erase(obj);
#endif
            this->_free_blocks.push_back(obj);
            obj->~C();
            ZHTTPD_ASSERT_UNLOCK(this->_mutex);
        }

        void clean()
        {
            unsigned int count = 0;
            typename std::vector<C*>::reverse_iterator it = this->_blocks.rbegin();
            typename std::vector<C*>::reverse_iterator itEnd = this->_blocks.rend();
            while (it != itEnd)
            {
                typename std::vector<C*>::reverse_iterator f = std::find(this->_free_blocks.rbegin(), this->_free_blocks.rend(), *it);
                if (f != this->_free_blocks.rend())
                {
                    ++count;
                    delete reinterpret_cast<char*>(*it);
                    this->_blocks.erase(it.base() - 1);
                    ++it;
                    this->_free_blocks.erase(f.base() - 1);
                }
                else
                    ++it;
            }
            if (!this->_free_blocks.empty())
            {
                LOG_WARN("Double free detected...");
                this->_free_blocks.clear();
            }
            StatsManager::getInstance()->addMemory(-count * sizeof(C));
        }

    };
}

#endif

