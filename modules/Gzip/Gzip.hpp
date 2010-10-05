#ifndef __GZIP_HPP__
#define __GZIP_HPP__

#include <zlib.h>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModuleManager.hpp"
#include "api/IModule.hpp"

class Gzip : public ZHTTPD::API::IModule
{
    private:
        enum
        {
            GZIP_CHUNK = 16384
        };
        bool _load(ZHTTPD::API::IRequest* request);
        void _unload();
        bool _deflate(ZHTTPD::API::IBuffer* in_buffer, ZHTTPD::API::IBuffer* out_buffer);
        bool _initialized;
        z_stream _stream;
    public:
        Gzip(ZHTTPD::API::IModuleManager* manager);
        ~Gzip();
        bool processRequest(ZHTTPD::API::EVENT::Type event, ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buffer);
};

#endif

