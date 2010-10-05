#ifndef __FILEREADER_HPP__
# define __FILEREADER_HPP__

# include <fstream>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModuleManager.hpp"
# include "api/IModule.hpp"

namespace ZHTTPD
{
    namespace MOD
    {

        class FileReaderManager;

        class FileReader : public ZHTTPD::API::IModule
        {
            private:
                FileReaderManager* _manager;
                std::ifstream _file;
                void _onEnd(ZHTTPD::API::IRequest* request);
                void _onIdle(ZHTTPD::API::IRequest* request);
                std::string const& _findMimeType(std::string const& file);
                void _setRanges(std::string const& str);
                void _writeRanges(ZHTTPD::API::IRequest* request);
#ifdef ZHTTPD_DEBUG
                unsigned int _total_bytes;
#endif
                unsigned int _file_size;
                unsigned int _range_begin;
                unsigned int _range_end;
            public:
                FileReader(API::IModuleManager* manager);
                ~FileReader();
                bool processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer);
        };

    }
}

#endif

