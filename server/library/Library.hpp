
#ifndef __LIBRARY_HPP__
# define __LIBRARY_HPP__

# include <string>

# ifdef WIN32
#  include "LibraryWindows.hpp"
# else
#  include "LibraryUnix.hpp"
# endif

namespace zhttpd
{
    class Library : public implementation::Library
    {
    private:
        std::string _path;

    public:
        Library(std::string const& path) : implementation::Library(path), _path(path)
        {
        }

        std::string const& getPath()
        {
            return this->_path;
        }

        template<typename Cast>
        Cast resolve(char const* name)
        {
            return reinterpret_cast<Cast>(implementation::Library::resolve(name));
        }
    };
}

#endif /* !__LIBRARY_HPP__ */

