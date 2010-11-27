#include "PreOutputBuilder.hpp"
#include "PreOutputBuilderManager.hpp"

namespace ZHTTPD
{
    namespace MOD
    {

        void my_putnbr_base_rec(std::string& out, unsigned int nbr, int base, const char *str)
        {
            if (nbr > 0)
            {
                my_putnbr_base_rec(out, nbr / base, base, str);
                out += str[nbr % base];
            }
        }

        void my_putnbr_base(std::string& out, unsigned int nbr, int base, const char *str)
        {
            if (nbr == 0)
                out += str[0];
            else
                my_putnbr_base_rec(out, nbr, base, str);
        }

        PreOutputBuilder::PreOutputBuilder(API::IModuleManager* manager) :
            _manager(reinterpret_cast<PreOutputBuilderManager*>(manager)),
            _chunked(false),
            _headers(false)
        {
        }

        PreOutputBuilder::~PreOutputBuilder()
        {
        }

        void PreOutputBuilder::_sendHeaders(API::IRequest* request, bool has_data /* = true */)
        {
            std::string buffer;
            buffer += "HTTP/1.1 ";
            if (request->getResponseCode() == ZHTTPD::API::HTTP_CODE::UNDEFINED)
                request->setResponseCode(ZHTTPD::API::HTTP_CODE::OK);
            my_putnbr_base(buffer, request->getResponseCode(), 10, "0123456789");
            buffer += " ";
            buffer += request->getResponseMessage();
            buffer += "\r\n";
            if (request->getRequestHeader("Connection") != "close")
            {
                request->setResponseHeader("Connection", "Keep-Alive");
                request->setResponseHeader("Keep-Alive", "timeout=3, max=50");
            }
            std::list<std::string const*> keys = request->getResponseHeaderKeys();
            std::list<std::string const*>::iterator it = keys.begin();
            std::list<std::string const*>::iterator itEnd = keys.end();
            for (; it != itEnd; ++it)
            {
                std::string const* key = *it;
                std::string const& val = request->getResponseHeader(*key);
                if (val.size() > 0)
                    buffer += *key + ": " + val + "\r\n";
            }
            if (has_data && request->getResponseHeader("Content-Length") == "")
            {
                buffer += "Transfer-Encoding: chunked\r\n";
                this->_chunked = true;
            }
            else
                buffer += "\r\n";
            request->giveData(request->getBufferManager().allocate(buffer.data(), buffer.size()));
            this->_headers = true;
        }

        void PreOutputBuilder::_sendChunkInfo(API::IRequest* request, API::IBuffer* buffer)
        {
            std::string buf;
            buf += "\r\n";
            my_putnbr_base(buf, buffer->getSize(), 16, "0123456789abcdef");
            buf += "\r\n";
            request->giveData(request->getBufferManager().allocate(buf.data(), buf.size()));
        }

        bool PreOutputBuilder::processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer)
        {
            if (event == API::EVENT::ON_RESPONSE_DATA)
            {
                if (!this->_headers)
                    this->_sendHeaders(request);
                if (this->_chunked)
                    this->_sendChunkInfo(request, buffer);
                request->giveData(buffer);
                return true;
            }
            else if (event == API::EVENT::ON_END)
            {
                if (!this->_headers)
                    this->_sendHeaders(request, false);
                else if (this->_chunked)
                    request->giveData(request->getBufferManager().allocate("\r\n0\r\n\r\n")); // final chunk
                request->raiseEnd();
                return true;
            }
            return false;
        }

    }
}

