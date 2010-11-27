#ifndef __FILEREADER_HPP__
# define __FILEREADER_HPP__

# include <fstream>

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModuleManager.hpp"
# include "api/IModule.hpp"

namespace zhttpd
{
    namespace mod
    {

        class FileReaderManager;

        class FileReader : public zhttpd::api::IModule
        {
            private:
                FileReaderManager* _manager;
                std::ifstream _file;
                void _onEnd(zhttpd::api::IRequest* request);
                void _onIdle(zhttpd::api::IRequest* request);
                std::string const& _findMimeType(std::string const& file);
                void _setRanges(std::string const& str);
                void _writeRanges(zhttpd::api::IRequest* request);
#ifdef ZHTTPD_DEBUG
                unsigned int _total_bytes;
#endif
                unsigned int _file_size;
                unsigned int _range_begin;
                unsigned int _range_end;
            public:
                FileReader(api::IModuleManager* manager);
                ~FileReader();
                bool processRequest(api::event::Type event, api::IRequest* request, api::IBuffer* buffer);
        };

    }
}

#endif

