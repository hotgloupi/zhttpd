
#ifndef __PATH_HPP__
# define __PATH_HPP__

# include <string>
# include <list>
# include <set>

# ifdef WIN32
#  include "PathWindows.hpp"
# else
#  include "PathUnix.hpp"
# endif

namespace zhttpd
{
    class Path : public implementation::Path
    {
    private:
        std::string _path;

    public:
        Path(std::string const& path);
        Path(Path const& p);
        Path const& operator =(Path const& p);
        Path const& operator =(std::string const& p);
        bool exists() const;
        bool isDirectory() const;
        std::string getAbsolutePath() const;
        bool isAbsolute() const;
        std::string getBasePath() const;
        std::string getFileName() const;
        std::string getFileExtension() const;

        std::list<std::string> getDirectoryContent() const;
        std::set<std::string> getDirectoryContentSet() const;

        Path const& operator +=(Path const& right);
        Path const& operator +=(std::string const& right);
        Path operator +(std::string const& right);

        operator std::string()
        {
            return this->_path;
        }

        static inline bool exists(std::string const& path)
        {
            return implementation::Path::exists(path);
        }

        static inline bool isDirectory(std::string const& path)
        {
            return implementation::Path::isDirectory(path);
        }

        static inline std::list<std::string> getDirectoryContent(std::string const& path)
        {
            return implementation::Path::getDirectoryContent(path);
        }
        static inline std::string cwd()
        {
            return implementation::Path::cwd();
        }

        static inline bool isAbsolute(std::string const& path)
        {
            return implementation::Path::isAbsolute(path);
        }

    private:
        void _clean();
    };
}

#endif // PATH_HPP__

