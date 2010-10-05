#include <istream>
#include <sstream>

#include <iostream>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"

#include "FileReader.hpp"
#include "FileReaderManager.hpp"

#include "utils/Logger.hpp"

namespace ZHTTPD
{
    namespace MOD
    {

        FileReader::FileReader(API::IModuleManager* manager) :
            _manager(reinterpret_cast<FileReaderManager*>(manager))
#ifdef ZHTTPD_DEBUG
            , _total_bytes(0)
#endif
            , _file_size(0), _range_begin(0), _range_end(0)
        {
        }

        FileReader::~FileReader()
        {
        }

        bool FileReader::processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer*)
        {
            if (event == API::EVENT::ON_END)
            {
                this->_onEnd(request);
                return true;
            }
            else if (event == API::EVENT::ON_IDLE)
            {
                this->_onIdle(request);
                return true;
            }
            return false;
        }

        void FileReader::_onIdle(ZHTTPD::API::IRequest* request)
        {
            if (this->_file.tellg() >= this->_range_end)
            {
#ifdef ZHTTPD_DEBUG
                LOG_DEBUG("Wrote " + Logger::toString(this->_total_bytes) + " bytes.");
#endif
                request->raiseEnd();
            }
            else if (this->_file.good())
            {
                unsigned int read_size;
                if (this->_range_end - this->_file.tellg() > 4096)
                    read_size = 4096;
                else
                    read_size = this->_range_end - this->_file.tellg() + 1;
                ZHTTPD::API::IBuffer* buf = request->getBufferManager().allocate(read_size);
                this->_file.read(buf->getRawData(), read_size);
#ifdef ZHTTPD_DEBUG
                this->_total_bytes += buf->getSize();
#endif
                request->giveData(buf);
                request->callLater(this->_manager->getDelay());
            }
            else if (this->_file.eof())
            {
                LOG_DEBUG("Wrote " + Logger::toString(this->_total_bytes) + " bytes.");
                request->raiseEnd();
            }
            else
                request->raiseError(API::HTTP_CODE::INTERNAL_SERVER_ERROR);
        }

        std::string const& FileReader::_findMimeType(std::string const& file)
        {
            std::string ext;
            std::string::const_reverse_iterator it = file.rbegin();
            std::string::const_reverse_iterator itEnd = file.rend();
            for (; it != itEnd; ++it)
                if (*it == '.')
                {
                    LOG_DEBUG("Using mime type " + this->_manager->getMimeType(ext));
                    return this->_manager->getMimeType(ext);
                }
                else
                    ext.insert(ext.begin(), *it);
            LOG_DEBUG("Using default mime type");
            return this->_manager->getDefaultMimeType();
        }

        void FileReader::_setRanges(std::string const& str)
        {
            bool equal = false;
            bool minus = false;
            std::string range_begin;
            std::string range_end;
            std::string::const_iterator it = str.begin();
            std::string::const_iterator itEnd = str.end();
            for (; it != itEnd; ++it)
                if (*it == '=')
                    equal = true;
                else if (*it == '-')
                    minus = true;
                else if (equal && minus)
                    range_end.push_back(*it);
                else if (equal)
                    range_begin.push_back(*it);
            if (range_begin.size() > 0)
            {
                std::stringstream ss;
                ss << range_begin;
                ss >> this->_range_begin;
                if (range_end.size() > 0)
                {
                    ss.clear();
                    ss << range_end;
                    ss >> this->_range_end;
                    if (this->_range_end >= this->_file_size)
                        this->_range_end = this->_file_size - 1;
                }
                else
                    this->_range_end = this->_file_size - 1;
                if (this->_range_begin >= this->_range_end)
                    this->_range_begin = 0;
            }
        }

        void FileReader::_writeRanges(ZHTTPD::API::IRequest* request)
        {
            if (this->_range_begin != 0 || this->_range_end != 0)
            {
                if (this->_file.good())
                    this->_file.seekg(this->_range_begin, std::ios::beg);
                request->setResponseCode(API::HTTP_CODE::PARTIAL_CONTENT);
            }
            else
            {
                request->setResponseCode(API::HTTP_CODE::OK);
                this->_range_begin = 0;
                this->_range_end = this->_file_size - 1;
            }
            std::stringstream ss;
            ss << "bytes " << this->_range_begin << "-" << this->_range_end << "/" << this->_file_size;
            request->setResponseHeader("Content-Range", ss.str());
        }

        void FileReader::_onEnd(ZHTTPD::API::IRequest* request)
        {
            if (this->_file.is_open())
                return;
            this->_file.open(request->getFilePath().c_str());
            if (this->_file.good())
            {
                this->_file.seekg(0, std::ios::end);
                this->_file_size = this->_file.tellg();
                this->_file.seekg(0, std::ios::beg);
                request->setResponseHeader("Content-Length", "0");
                std::string type = this->_findMimeType(request->getFilePath());
                if (type != "")
                    request->setResponseHeader("Content-Type", type);
                request->setResponseHeader("Accept-Ranges", "bytes");
                if (this->_file_size == 0)
                    request->raiseEnd();
                else
                {
                    this->_setRanges(request->getRequestHeader("Range"));
                    this->_writeRanges(request);
                    std::stringstream ss;
                    ss << this->_range_end - this->_range_begin + 1;
                    request->setResponseHeader("Content-Length", ss.str());
                    this->_onIdle(request);
                }
            }
            else
            {
                LOG_INFO("File \"" + request->getFilePath() + "\" not found.");
                request->raiseError(API::HTTP_CODE::NOT_FOUND);
            }
        }
    }
}

