
#ifndef __ABSTRACTMANAGER_HPP__
# define __ABSTRACTMANAGER_HPP__

# include <stdexcept>
# include <map>
# include <string>

# include "api/IModuleManager.hpp"
# include "utils/MemoryPool.hpp"
# include "thread/ScopeLock.hpp"
# include "thread/Mutex.hpp"

namespace zhttpd
{
    namespace mod
    {
        namespace POLICIES
        {
            class NoConfigurationPolicy : public zhttpd::api::IModuleManager
            {
            public:
                virtual void addConfigurationEntry(std::string const&, std::string const&)
                {
                }
            };

            class MapConfigurationPolicy : public zhttpd::api::IModuleManager
            {
            protected:
                std::map<std::string, std::string> _configuration;

            public:
                virtual void addConfigurationEntry(std::string const& key, std::string const& value)
                {
                    this->_configuration[key] = value;
                }

                std::map<std::string, std::string> const& getConfiguration() const
                {
                    return this->_configuration;
                }

                std::string const& getConfigurationValue(std::string const& key) const
                {
                    std::map<std::string, std::string>::const_iterator item = this->_configuration.find(key);
                    if (item == this->_configuration.end())
                        throw std::runtime_error("\"" + key + "\" does not exists");
                    return item->second;
                }
            };

            template<typename ManagedClass, typename ConfigurationPolicy>
            class StatefullManagementPolicy : public ConfigurationPolicy
            {
                private:
                    MemoryPool<ManagedClass>    _memory_pool;
                    Mutex                       _memory_mutex;
#ifdef ZHTTPD_DEBUG
                    std::set<zhttpd::api::IModule*> _allocated_modules;
#endif

                public:
                    virtual zhttpd::api::IModule* getInstance(bool in_response = false)
                    {
                        (void) in_response;
                        this->_memory_mutex.lock();
                        zhttpd::api::IModule* module = this->_memory_pool.allocate(this);
#ifdef ZHTTPD_DEBUG
                        this->_allocated_modules.insert(static_cast<zhttpd::api::IModule*>(module));
#endif
                        this->_memory_mutex.unlock();
                        return module;
                    }
                    virtual void releaseInstance(zhttpd::api::IModule* module)
                    {
                        this->_memory_mutex.lock();
                        this->_memory_pool.release(reinterpret_cast<ManagedClass*>(module));
#ifdef ZHTTPD_DEBUG
                        this->_allocated_modules.erase(module);
#endif
                        this->_memory_mutex.unlock();
                    }

#ifdef ZHTTPD_DEBUG
                    virtual ~StatefullManagementPolicy()
                    {
                        if (this->_allocated_modules.size() > 0)
                        {
                            LOG_WARN("Not all modules are released: " +
                                     Logger::toString(this->_allocated_modules.size()) + " left");
                            std::set<zhttpd::api::IModule*>::iterator it = this->_allocated_modules.begin(),
                                                                   end = this->_allocated_modules.end();
                            for (; it != end; ++it)
                            {
                                this->_memory_pool.release(reinterpret_cast<ManagedClass*>(*it));
                            }
                        }
                    }
#endif
            };

            template<typename ManagedClass, typename ConfigurationPolicy>
            class StatelessManagementPolicy : public ConfigurationPolicy
            {
            private:
                api::IModule* _module;

            public:
                StatelessManagementPolicy() : _module(0)
                {
                    this->_module = new ManagedClass(this);
                }

                virtual ~StatelessManagementPolicy()
                {
                    if (this->_module != 0)
                    {
                        delete this->_module;
                        this->_module = 0;
                    }
                }

                virtual zhttpd::api::IModule* getInstance(bool in_response = false)
                {
                    (void) in_response;
                    return this->_module;
                }

                virtual void releaseInstance(zhttpd::api::IModule*) {}
            };
        }

        template<typename ManagementPolicy>
        class AbstractManager : public ManagementPolicy
        {
        private:
            std::string _name;
            zhttpd::api::category::Type _category;

        public:
            AbstractManager(std::string const& name,
                            zhttpd::api::category::Type category) :
                ManagementPolicy(), _name(name), _category(category)
            {
            }

            virtual std::string const& getName() const
            {
                return this->_name;
            }

            virtual zhttpd::api::category::Type getCategory() const
            {
                return this->_category;
            }

            virtual bool isRequired(zhttpd::api::IRequest const&) const
            {
                return true;
            }
        };

# define ZHTTPD_MOD_ABSTRACTMANAGER_HELPER(state)                                                              \
        template<typename ManagedClass, typename ConfigurationPolicy = POLICIES::NoConfigurationPolicy>     \
        class state##Manager :                                                                              \
            public AbstractManager< POLICIES::state##ManagementPolicy<ManagedClass, ConfigurationPolicy> >  \
        {                                                                                                   \
        public:                                                                                             \
            state##Manager(std::string const& name,                                                         \
                           zhttpd::api::category::Type category = zhttpd::api::category::UNDEFINED) :             \
                AbstractManager< POLICIES::state##ManagementPolicy<ManagedClass, ConfigurationPolicy> >(name, category) {}       \
        }

        ZHTTPD_MOD_ABSTRACTMANAGER_HELPER(Statefull);
        ZHTTPD_MOD_ABSTRACTMANAGER_HELPER(Stateless);
#undef ZHTTPD_MOD_ABSTRACTMANAGER_HELPER

    }
}

#endif /* !__ABSTRACTMANAGER_HPP__ */

