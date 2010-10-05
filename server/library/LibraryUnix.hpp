
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
                this->_handle = ::dlopen((path).c_str(), RTLD_NOW);
                char const* err = ::dlerror();
                if (this->_handle == 0)
                {
                    if (err != 0)
                        throw std::runtime_error("Cannot open library " + path + ": " + std::string(err));
                    else
                        throw std::runtime_error("Cannot open library " + path);
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

