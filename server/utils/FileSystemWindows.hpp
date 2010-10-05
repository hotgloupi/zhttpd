
#ifndef __FILESYSTEMWINDOWS_HPP__
# define __FILESYSTEMWINDOWS_HPP__

#include <list>
#include <string>
#include "FileType.hpp"

namespace ZHTTPD
{
    namespace IMPLEMENTATION
    {
        class FileSystem
        {
        protected:
            bool exists(std::string const& path) const;
            FileType getType(std::string const& path) const;
			std::list<std::string> getFileList(std::string const& path) const;

        public:
            static std::string cwd();

            FileSystem();
            virtual ~FileSystem();
        };
    }
}

#endif // __FILESYSTEMWINDOWS_HPP__

