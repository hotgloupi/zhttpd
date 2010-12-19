#ifndef __GZIP_HPP__
#define __GZIP_HPP__

#include <zlib.h>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModuleManager.hpp"
#include "api/IModule.hpp"

class Gzip : public zhttpd::api::IModule
{
    private:
        enum
        {
            GZIP_CHUNK = 16384
        };
        bool _load(zhttpd::api::IRequest* request);
        void _unload();
        bool _deflate(zhttpd::api::IBuffer* in_buffer, zhttpd::api::IBuffer* out_buffer);
        bool _initialized;
        z_stream _stream;
    public:
        Gzip(zhttpd::api::IModuleManager* manager);
        ~Gzip();
        bool processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer);
};

#endif

