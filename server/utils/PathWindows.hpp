
#ifndef __PATHWINDOWS_HPP__
# define __PATHWINDOWS_HPP__

# include <Windows.h>

# include <string>
# include <list>

namespace zhttpd
{
    namespace implementation
    {
        class Path
        {
        public:
            static inline bool exists(std::string const& path)
            {
                DWORD res = GetFileAttributes(path.c_str());
                if (INVALID_FILE_ATTRIBUTES == res)
                    return false;
                return true;
            }

            static inline bool isDirectory(std::string const& path)
            {
                DWORD res = GetFileAttributes(path.c_str());
                if (FILE_ATTRIBUTE_DIRECTORY & res)
                    return true;
                return false;
            }

            static inline std::list<std::string> getDirectoryContent(std::string const& path)
            {
                std::list<std::string> list;
                WIN32_FIND_DATA file;
                std::string tmp = path + "/*";
                HANDLE dir = ::FindFirstFile(tmp.c_str(), &file);
                if (dir != INVALID_HANDLE_VALUE)
                {
                    do
                    {
                        list.push_back(file.cFileName);
                    } while(::FindNextFile(dir, &file));
                    ::FindClose(dir);
                }
	            return list;
            }

            static inline std::string cwd()
            {
                char buf[MAX_PATH];
                int len = ::GetCurrentDirectory(MAX_PATH, buf);
                return std::string(buf, len);
            }

            static inline bool isAbsolute(std::string const& path)
            {
                return path.size() > 2 && path[1] == ':' && path[0] >= 'A' && path[0] <= 'Z';
            }
        };
    }
}

#endif /* !__PATHWINDOWS_HPP__ */

