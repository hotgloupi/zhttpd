
#ifndef __VHOSTPARSER_HPP__
# define __VHOSTPARSER_HPP__

# include "parser/ticpp/ticpp.h"
# include "configuration/VHost.hpp"
# include "configuration/Configuration.hpp"
# include "configuration/IVHostGuideline.hpp"

namespace zhttpd
{
    namespace parser
    {
        class VHostParser
        {
        private:
            typedef void (VHostParser::*hdlr)(ticpp::Node*, VHost*);
            std::map<std::string, hdlr> _hdlrs;

            void _parseUrl(ticpp::Node* node, VHost* vhost);
            void _parseDocumentRoot(ticpp::Node*, VHost* vhost);
            void _parseHeader(ticpp::Node* node, VHost* vhost);
            void _parseHost(ticpp::Node* node, VHost* vhost);
            void _parseListen(ticpp::Node* node, VHost* vhost);

        public:
            VHostParser();
            virtual ~VHostParser();

            void parseGuideLine(ticpp::Node* node, VHost* vhost);
            void parseVHost(ticpp::Node* node, Configuration* conf, VHost* parent = 0);
        };
    }
}

#endif // !__VHOSTPARSER_HPP__

