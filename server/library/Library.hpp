
#ifndef __LIBRARY_HPP__
# define __LIBRARY_HPP__

# include <string>

# ifdef WIN32
#  include "LibraryWindows.hpp"
# else
#  include "LibraryUnix.hpp"
# endif

namespace ZHTTPD
{
    class Library : public IMPLEMENTATION::Library
    {
    private:
        std::string _path;

    public:
        Library(std::string const& path) : IMPLEMENTATION::Library(path), _path(path)
        {
        }

        std::string const& getPath()
        {
            return this->_path;
        }

        template<typename Cast>
        Cast resolve(char const* name)
        {
            return reinterpret_cast<Cast>(IMPLEMENTATION::Library::resolve(name));
        }
    };
}

#endif /* !__LIBRARY_HPP__ */

