
#ifndef __MODULEPARSER_HPP__
# define __MODULEPARSER_HPP__

# include "parser/ticpp/ticpp.h"
# include "configuration/Configuration.hpp"
# include "configuration/VHost.hpp"

namespace ZHTTPD
{
    namespace PARSER
    {
        class ModuleParser
        {
        public:
            ModuleParser();
            virtual ~ModuleParser();

            void parseModule(ticpp::Node* node, Configuration* config);
            void parseModule(ticpp::Node* node, VHost* vhost, Configuration* config);
            void parseOption(ticpp::Node* node, ModuleConfiguration* module);
        };
    }
}

#endif // MODULEPARSER_HPP__
