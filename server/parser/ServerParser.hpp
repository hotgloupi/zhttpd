
#ifndef __SERVERPARSER_HPP__
# define __SERVERPARSER_HPP__

# include <map>

# include "parser/ticpp/ticpp.h"
# include "configuration/Configuration.hpp"

namespace zhttpd
{
    namespace parser
    {
        class ServerParser
        {
            typedef void (ServerParser::* parseConf)(ticpp::Node* node, Configuration* conf);

        private:
            std::map<std::string, parseConf> _confParsers;

            void _parseListen(ticpp::Node* node, Configuration* conf);
            void _parseModuleDirectory(ticpp::Node* node, Configuration* conf);
            void _parseIndexFiles(ticpp::Node* node, Configuration* conf);
            void _parseIndexFileName(ticpp::Node* node, Configuration* conf);

        public:
            ServerParser();
            virtual ~ServerParser();

            void parseServerConfNode(ticpp::Node* node, Configuration* conf);
        };
    }
}

#endif // SERVERPARSER_HPP__

