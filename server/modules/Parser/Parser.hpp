
#ifndef __PARSER_HPP__
# define __PARSER_HPP__

# include "api/constants.hpp"
# include "api/IBuffer.hpp"
# include "api/IRequest.hpp"
# include "api/IModule.hpp"

namespace zhttpd
{
    namespace mod
    {
        class ParserManager;

        class Parser : public zhttpd::api::IModule
        {
            enum {
                MAX_HEADER_SIZE = 10000
            };

        private:
            ParserManager*              _manager;
            unsigned int                _separator;
            bool                        _completed;
            api::size_t                 _content_length;
            api::size_t                 _received_length;
            api::size_t                 _header_length;
            zhttpd::api::IRequest*      _request;
            std::list<api::IBuffer*>    _buffers;

        public:
            Parser(api::IModuleManager* manager);
            ~Parser();
            bool processRequest(api::event::Type event, api::IRequest* request, api::IBuffer* buffer);

        private:
            api::size_t _parseRequest(api::IRequest* request) const;
            void _parseLine(api::IRequest* request, std::string const& line) const;
            void _parseMethod(api::IRequest* request, std::string const& line) const;
            void _giveData(api::IRequest* request, api::IBuffer* buffer);
        };

    }
}

#endif

