
#ifndef __LIBRARYWINDOWS_HPP__
# define __LIBRARYWINDOWS_HPP__

# include <string>
# include <stdexcept>

# include <direct.h>
# include <windows.h>

# include "utils/String.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class Library
        {
        private:
            typedef HINSTANCE handle_t;
            handle_t _handle;

        public:
            Library(std::string const& path)
            {
                this->_handle = ::LoadLibrary((path).c_str());
                if (this->_handle == 0)
                {
                    throw std::runtime_error(
                        std::string("LoadLibrary error: ") + ::GetLastError()
                    );
                }
            }

            virtual ~Library()
            {
                if (this->_handle != 0)
                {
                    ::FreeLibrary(this->_handle);
                    this->_handle = 0;
                }
            }

            virtual void* resolve(char const* name)
            {
                void* func = ::GetProcAddress(this->_handle, name);
                if (func == 0)
                {
                    throw std::runtime_error(
                        std::string("GetProcAddress error: ") + ::GetLastError()
                    );
                }
                return func;
            }
        };
    }
}

#endif /* !__LIBRARYWINDOWS_HPP__ */

