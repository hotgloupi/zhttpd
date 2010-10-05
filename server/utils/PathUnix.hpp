
#ifndef __PATHUNIX_HPP__
# define __PATHUNIX_HPP__

# include <string>
# include <list>

# include <sys/stat.h>
# include <dirent.h>
# include <cstring>

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class Path
        {
        public:
            static inline bool exists(std::string const& path)
            {
                struct stat file_stat;
                if (::stat(path.c_str(), &file_stat) != 0)
                    return false;
                return true;
            }

            static inline bool isDirectory(std::string const& path)
            {
                struct stat file_stat;
                if (::stat(path.c_str(), &file_stat) != 0)
                    return false;
                if (S_ISDIR(file_stat.st_mode))
                    return true;
                return false;
            }

            static inline std::list<std::string> getDirectoryContent(std::string const& path)
            {
                DIR* dir = ::opendir(path.c_str());
                if (dir == 0)
                    return std::list<std::string>();

                std::list<std::string> files;
                struct dirent* content;
                while ((content = ::readdir(dir)) != 0)
                {
                    if (::strcmp(content->d_name, ".") != 0 &&
                        ::strcmp(content->d_name, "..") != 0)
                    {
                        files.push_back(std::string(content->d_name));
                    }
                }
                ::closedir(dir);
                return files;
            }

            static inline std::string cwd()
            {
                char buf[4096];
                char* path = ::getcwd(buf, 4096);
                if (path != 0)
                {
                    return std::string(path);
                }
                return std::string();
            }

            static inline bool isAbsolute(std::string const& path)
            {
                return path.size() > 0 && path[0] == '/';
            }
        };
    }
}

#endif /* !__PATHUNIX_HPP__ */

