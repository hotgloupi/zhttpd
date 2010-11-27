
#include "api/types.hpp"
#include "Path.hpp"

using namespace zhttpd;

Path::Path(std::string const& path) : _path(path)
{
    this->_clean();
}

Path::Path(Path const& p) : _path(p._path)
{
    this->_clean();
}

Path const& Path::operator =(Path const& p)
{
    this->_path = p._path;
    this->_clean();
    return *this;
}

Path const& Path::operator =(std::string const& p)
{
    this->_path = p;
    this->_clean();
    return *this;
}

bool Path::isAbsolute() const
{
    return implementation::Path::isAbsolute(this->_path);
}

std::string Path::getAbsolutePath() const
{
    if (this->isAbsolute())
        return this->_path;
    return implementation::Path::cwd() + "/" + this->_path;
}

bool Path::exists() const
{
    return implementation::Path::exists(this->_path);
}

bool Path::isDirectory() const
{
    return implementation::Path::isDirectory(this->_path);
}

std::list<std::string> Path::getDirectoryContent() const
{
    return implementation::Path::getDirectoryContent(this->_path);
}

std::set<std::string> Path::getDirectoryContentSet() const
{
    std::list<std::string> content = this->getDirectoryContent();
    std::list<std::string>::iterator it = content.begin();
    std::list<std::string>::iterator end = content.end();
    std::set<std::string> res;
    for (; it != end; ++ it)
        res.insert(*it);
    return res;
}

std::string Path::getBasePath() const
{
    std::string res = this->getAbsolutePath();
    if (implementation::Path::isDirectory(res))
        return res;
    std::size_t last = res.find_last_of('/');
    return res.substr(0, last);
}

std::string Path::getFileName() const
{
    if (implementation::Path::isDirectory(this->_path))
        return "";
    std::size_t last = this->_path.find_last_of('/');
    return this->_path.substr(last + 1);
}

std::string Path::getFileExtension() const
{
    std::string filename = this->getFileName();
    std::size_t i_sep = filename.find_last_of('.');
    if (i_sep != std::string::npos)
        return filename.substr(i_sep + 1);
    return "";
}

Path const& Path::operator +=(Path const& right)
{
    if (this->_path.size() == 0)
    {
        *this = right;
        return *this;
    }
    else if (this->_path[this->_path.size() - 1] != '/')
    {
        this->_path += '/';
    }
    this->_path += right._path;
    this->_clean();
    return *this;
}

Path const& Path::operator +=(std::string const& right)
{
    if (this->_path.size() == 0)
    {
        *this = right;
        return *this;
    }
    else if (this->_path[this->_path.size() - 1] != '/')
    {
        this->_path += '/';
    }
    this->_path += right;
    this->_clean();
    return *this;
}


Path Path::operator +(std::string const& right)
{
    Path res(*this);
    res += right;
    return res;
}

void Path::_clean()
{
    std::size_t i = 0, end = this->_path.size();
    for (; i != end; ++i)
    {
        if (this->_path[i] == '\\')
            this->_path[i] = '/';
    }
}
