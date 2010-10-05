#ifndef __PARSER_HPP__
# define __PARSER_HPP__

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"

namespace ZHTTPD
{
    namespace MOD
    {
        class ParserManager;

        class Parser : public ZHTTPD::API::IModule
        {
        private:
            enum
            {
                MAX_HEADER_SIZE = 10000
            };

            ParserManager* _manager;
            unsigned int _separator;
            bool _completed;
            API::size_t _content_length;
            API::size_t _received_length;
            API::size_t _header_length;
            ZHTTPD::API::IRequest* _request;
            std::list<API::IBuffer*> _buffers;

        public:
            Parser(API::IModuleManager* manager);
            ~Parser();
            bool processRequest(API::EVENT::Type event, API::IRequest* request, API::IBuffer* buffer);

        private:
            API::size_t _parseRequest(API::IRequest* request) const;
            void _parseLine(API::IRequest* request, std::string const& line) const;
            void _parseMethod(API::IRequest* request, std::string const& line) const;
            void _giveData(API::IRequest* request, API::IBuffer* buffer);
        };

    }
}

#endif

