
#ifdef WIN32

# include <Windows.h>

# include "utils/Logger.hpp"
# include "utils/String.hpp"
# include "FileSystemWindows.hpp"
# include "FileType.hpp"

using namespace zhttpd;
using namespace zhttpd::implementation;

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem()
{

}

bool FileSystem::exists(std::string const& path) const
{
    DWORD res = GetFileAttributes(path.c_str());
    if (INVALID_FILE_ATTRIBUTES == res)
        return false;
    return true;
}

FileType FileSystem::getType(std::string const& path) const
{
    DWORD res = ::GetFileAttributes(path.c_str());
    if (INVALID_FILE_ATTRIBUTES != res)
    {
        if (res & FILE_ATTRIBUTE_DIRECTORY)
            return DIRECTORY;
        else if (res & (FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE))
            return FILE;
    }
    else
        LOG_DEBUG("GetFileAttributes() fail (" + ::GetLastError() + ")");
    return NOT_A_FILE;
}

std::list<std::string> FileSystem::getFileList(std::string const& path) const
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
    int err = ::GetLastError();
	return list;
}

std::string FileSystem::cwd()
{
    char buf[MAX_PATH];
    int len = ::GetCurrentDirectory(MAX_PATH, buf);
    return std::string(buf, len);
}

#endif // !WIN32
