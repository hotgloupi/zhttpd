
#ifndef __LIBRARYUNIX_HPP__
# define __LIBRARYUNIX_HPP__

# include <string>
# include <stdexcept>

# include <sys/types.h>
# include <dlfcn.h>

# include "utils/Logger.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class Library
        {
        private:
            typedef void* handle_t;
            handle_t _handle;

        public:
            Library(std::string const& path)
            {
                char const* c_path = path.c_str();
                this->_handle = ::dlopen(c_path, RTLD_NOW);
                char const* err = ::dlerror();

                if (this->_handle == 0)
                {
                    if (err != 0)
                        throw std::runtime_error("Cannot open library " + path + ": " + std::string(err));
                    else
                        throw std::runtime_error("Cannot open library " + path);
                }
                else if (err != 0)
                {
                    LOG_WARN("dlopen(): " + std::string(err));
                }
            }

            virtual ~Library()
            {
                if (this->_handle != 0)
                {
                    ::dlclose(this->_handle);
                    this->_handle = 0;
                }
            }

            virtual void* resolve(char const* name)
            {
                ::dlerror();
                void* func = ::dlsym(this->_handle, name);
                char const* err = ::dlerror();
                if (err != 0)
                    throw std::runtime_error(err);
                return func;
            }
        };
    }
}

#endif /* !__LIBRARYUNIX_HPP__ */

