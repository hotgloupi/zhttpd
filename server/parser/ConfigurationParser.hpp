
#ifndef __CONFIGURATIONPARSER_HPP__
# define __CONFIGURATIONPARSER_HPP__

# include <string>

# include "configuration/Configuration.hpp"
# include "parser/ticpp/ticpp.h"

namespace zhttpd
{
    namespace parser
    {
        class ConfigurationParser
        {
        private:
            ticpp::Document* _xml_doc;

        public:
            ConfigurationParser();
            virtual ~ConfigurationParser();

            void loadFile(std::string const& filename);
            void loadString(std::string const& xml);
            Configuration* parse();

        private:
            void parseBaseModules(Configuration* config);
            void parseVHosts(Configuration* config);
            void parseServer(Configuration* config);
        };
    }
}

#endif // CONFPARSER_HPP__

