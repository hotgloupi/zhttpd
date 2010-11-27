
#include "utils/Logger.hpp"
#include "utils/Path.hpp"

#include "FileReaderManager.hpp"

using namespace zhttpd::mod;

FileReaderManager::FileReaderManager() : StatefullManager<FileReader>("mod_filereader"), _delay(0)
{
    this->_defaultMimeType = "";
}

FileReaderManager::~FileReaderManager()
{
}

unsigned int FileReaderManager::getDelay() const
{
    return this->_delay;
}

std::string const& FileReaderManager::getDefaultMimeType() const
{
    return this->_defaultMimeType;
}

std::string const& FileReaderManager::getMimeType(std::string const& ext) const
{
    std::map<std::string, std::string>::const_iterator it = this->_types.find(ext);
    if (it == this->_types.end())
        return this->_defaultMimeType;
    return it->second;
}

void FileReaderManager::addConfigurationEntry(std::string const& key, std::string const& value)
{
#ifdef ZHTTPD_DEBUG
    LOG_DEBUG("Adding mime type " + value + " for extension " + key);
#endif
    if (key.size() > 0)
    {
        if (key[0] == '.')
        {
            std::string str = key;
            str.erase(str.begin());
            this->_types[str] = value;
        }
        else if (key == "delay")
        {
            std::stringstream ss;
            ss << value;
            ss >> this->_delay;
        }
        else
            LOG_WARN("Unknown filereader option " + key + " = " + value);
    }
}

zhttpd::api::category::Type FileReaderManager::getCategory() const
{
    return zhttpd::api::category::PROCESSING;
}

bool FileReaderManager::isRequired(zhttpd::api::IRequest const& req) const
{
    return !zhttpd::Path::isDirectory(req.getFilePath());
}

