#include <string.h> // memcpy

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "utils/Logger.hpp"

#include "Gzip.hpp"

Gzip::Gzip(ZHTTPD::API::IModuleManager*) :
    _initialized(false)
{
}

Gzip::~Gzip()
{
    if (this->_initialized)
        this->_unload();
}

bool Gzip::_load(ZHTTPD::API::IRequest* request)
{
    this->_stream.zalloc = Z_NULL;
    this->_stream.zfree = Z_NULL;
    this->_stream.opaque = Z_NULL;
    if (deflateInit2(&this->_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK)
    {
        this->_initialized = true;
        request->setResponseHeader("Content-Length", "");
        request->setResponseHeader("Content-Encoding", "gzip");
        return true;
    }
    return false;
}

void Gzip::_unload()
{
    deflateEnd(&this->_stream);
    this->_initialized = false;
}

bool Gzip::_deflate(ZHTTPD::API::IBuffer* in_buffer, ZHTTPD::API::IBuffer* out_buffer)
{
    unsigned int bytes = 1;
    unsigned int total_bytes = 0;
    unsigned char out[GZIP_CHUNK];
    this->_stream.avail_in = in_buffer->getSize();
    this->_stream.next_in = reinterpret_cast<unsigned char*>(in_buffer->getRawData());
    while (true)
    {
        this->_stream.avail_out = GZIP_CHUNK;
        this->_stream.next_out = out;
        if (deflate(&this->_stream, Z_SYNC_FLUSH) == Z_STREAM_ERROR)
            return false;
        bytes = GZIP_CHUNK - this->_stream.avail_out;
        if (bytes > 0)
        {
            out_buffer->setSize(total_bytes + bytes);
            ::memcpy(out_buffer->getRawData() + total_bytes, out, bytes);
            total_bytes += bytes;
        }
        else
            break;
    }
    return true;
}

bool Gzip::processRequest(ZHTTPD::API::EVENT::Type event, ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* in_buffer)
{
    if (event == ZHTTPD::API::EVENT::ON_END)
    {
        if (this->_initialized)
            this->_unload();
        request->raiseEnd();
        return true;
    }
    else if (event == ZHTTPD::API::EVENT::ON_RESPONSE_DATA)
    {
        if (!this->_initialized && !this->_load(request))
            request->raiseError(ZHTTPD::API::HTTP_CODE::INTERNAL_SERVER_ERROR);
        else
        {
            ZHTTPD::API::IBuffer* out_buffer = request->getBufferManager().allocate(1);
            if (this->_deflate(in_buffer, out_buffer))
                request->giveData(out_buffer);
            else
            {
                request->raiseError(ZHTTPD::API::HTTP_CODE::INTERNAL_SERVER_ERROR);
                request->getBufferManager().release(out_buffer);
            }
        }
        request->getBufferManager().release(in_buffer);
        return true;
    }
    return false;
}

