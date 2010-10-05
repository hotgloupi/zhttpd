
#ifndef TEST_PARSER_HPP__
# define TEST_PARSER_HPP__

# include "Suite.hpp"
# include "parser/ConfigurationParser.hpp"
# include "parser/ConfigurationParserException.hpp"
# include "configuration/Configuration.hpp"
# include "utils/Logger.hpp"

class Test_Parser : public Test::Suite<Test_Parser>
{
private:
    /*
     * Load
     */

    void _loadFileingValidFile()
    {
        try
        {
            ZHTTPD::PARSER::ConfigurationParser p;
            p.loadFile("example/sample_conf.xml");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            this->assert_test(1 == 0, e.what());
        }
    }

    void _loadFileingBadFile()
    {
         try
        {
            ZHTTPD::PARSER::ConfigurationParser p;
            p.loadFile("osef.xml");
            this->assert_test(1 == 0, "Loading should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void _loadFileingBadFormatFile()
    {
        try
        {
            ZHTTPD::PARSER::ConfigurationParser p;
            p.loadFile("example/bad_format.xml");
            this->assert_test(1 == 0, "Loading should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    /*
     * Modules
     */

    void _parseBaseModules()
    {
        try
        {
            ZHTTPD::PARSER::ConfigurationParser p;
            p.loadFile("example/base_module.xml");
            ZHTTPD::Configuration* conf = p.parse();
            this->assert_test(conf != 0, "Configuration have not been initialized correctly");
            if (conf != 0)
            {
                //this->assert_test(conf->getModules().front()->getName() == "ssl", "Module have not been initialized correctly");
                //this->assert_test(conf->getModules().front()->getConfigurationEntry("version") == "3", "Options have not been initialized correctly");
                //this->assert_test(conf->getModules().front()->getConfigurationEntry("toto") == "", "Asking an undefined option does not return \"\"");
            }
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            this->assert_test(1 == 0, e.what());
        }
    }

    void _parseNoLoaded()
    {
        try
        {
            ZHTTPD::PARSER::ConfigurationParser p;
            p.parse();
            this->assert_test(1 == 0, "Parse should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void _parseBaseModulesMisformed()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/bad_format_module.xml");
            p.parse();
            this->assert_test(1 == 0, "parseBaseModules should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void _parseBaseModulesEmpty()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/no_base_modules.xml");
            p.parse();
            this->assert_test(1 == 0, "parseBaseModules should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void _parseBadModuleOption()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/bad_format_module_option.xml");
            p.parse();
            this->assert_test(1 == 0, "parseOption in module should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    /*
     * Mimetypes
     */

    void _parseBaseMimeTypes()
    {
        try
        {
            ZHTTPD::PARSER::ConfigurationParser p;
            p.loadFile("example/base_mimetypes.xml");
            p.parse();
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            this->assert_test(1 == 0, e.what());
        }
    }

    void _parseBaseMimeTypesEmpty()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/base_mimetypes_empty.xml");
            p.parse();
            this->assert_test(false, "parseBaseMimeTypes should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    /*
     * VHosts
     */

    void _parseVHosts()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/sample_conf.xml");
            p.parse();
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            this->assert_test(false, e.what());
        }
    }

    void _parseVHostsEmpty()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/vhosts_empty.xml");
            p.parse();
            this->assert_test(false, "parseBaseMimeTypes should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void _parseVHostsModule()
    {
        this->assert_test(false, "test unitaire module a faire");
    }

    /*
     *  Server
     */

    void _parseServer()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/base_server_conf.xml");
            p.parse();
            // ZHTTPD::Configuration* conf = p.getConfig();
            // this->assert_test(conf->getListenPorts()[8080] == "", "wrong port result: " + ZHTTPD::Logger::toString(8080) + " != ''");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            this->assert_test(false, e.what());
        }
    }

    void _parseServerEmpty()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/server_empty.xml");
            p.parse();
            this->assert_test(false, "parse server should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void _parseServerWrongOption()
    {
        ZHTTPD::PARSER::ConfigurationParser p;
        try
        {
            p.loadFile("example/unknown_option_server_conf.xml");
            p.parse();
            this->assert_test(false, "parse server should have failed, but succeed");
        }
        catch (ZHTTPD::PARSER::ConfigurationParserException& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

public:
    Test_Parser() : Test::Suite<Test_Parser>("Parser tests")
    {
        // Load
        this->addTestFunction(&Test_Parser::_loadFileingValidFile, "loadFileing valid file");
        this->addTestFunction(&Test_Parser::_loadFileingBadFile, "loadFileing unexisting file");
        this->addTestFunction(&Test_Parser::_loadFileingBadFormatFile, "loadFileing bad formatted file");

        // Modules
        this->addTestFunction(&Test_Parser::_parseBaseModules, "parsing base modules");
        this->addTestFunction(&Test_Parser::_parseNoLoaded, "parsing without loadFile");
        this->addTestFunction(&Test_Parser::_parseBaseModulesMisformed, "parsing base modules without name");
        this->addTestFunction(&Test_Parser::_parseBaseModulesEmpty, "parsing base modules without <modules> in configuration file");
        this->addTestFunction(&Test_Parser::_parseBadModuleOption, "parsing module option with a missing attribute");

        // MimeTypes
        // this->addTestFunction(&Test_Parser::_parseBaseMimeTypes, "parsing base mimetypes");
        // this->addTestFunction(&Test_Parser::_parseBaseMimeTypesEmpty, "parsing base mimetypes without <mimetypes>");

        // VHosts
        this->addTestFunction(&Test_Parser::_parseVHosts, "parsing vhosts");
        this->addTestFunction(&Test_Parser::_parseVHostsEmpty, "parsing vhosts, without <vhosts> in configuration file");

        // Server
        this->addTestFunction(&Test_Parser::_parseServer, "parsing server configuration");
        this->addTestFunction(&Test_Parser::_parseServerEmpty, "parsing server configuration, without <server> in configuration file");
        this->addTestFunction(&Test_Parser::_parseServerWrongOption, "parsing server configuration with wrong option");
    }
};

#endif // TEST_PARSER_HPP__

