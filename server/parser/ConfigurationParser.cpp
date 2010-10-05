
#include "utils/macros.hpp"
#include "ConfigurationParser.hpp"
#include "ConfigurationParserException.hpp"
#include "ServerParser.hpp"
#include "ModuleParser.hpp"
#include "VHostParser.hpp"

using namespace ZHTTPD::PARSER;

ConfigurationParser::ConfigurationParser() : _xml_doc(0)
{

}

ConfigurationParser::~ConfigurationParser()
{
    ZHTTPD_DELETE(this->_xml_doc);
}

void ConfigurationParser::loadFile(std::string const& filename)
{
    try
    {
        ZHTTPD_DELETE(this->_xml_doc);
        this->_xml_doc = new ticpp::Document(filename);
        this->_xml_doc->LoadFile();
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Fail to load configuration: " + filename);
    }
}

void ConfigurationParser::loadString(std::string const& xml)
{
    try
    {
        ZHTTPD_DELETE(this->_xml_doc);
        this->_xml_doc = new ticpp::Document();
        this->_xml_doc->Parse(xml);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Fail to load default configuration");
    }
}

ZHTTPD::Configuration* ConfigurationParser::parse()
{
    if (this->_xml_doc != 0)
    {
        Configuration* config = new Configuration;
        try
        {
            this->parseServer(config);
            this->parseBaseModules(config);
            this->parseVHosts(config);
            return config;
        }
        catch (ConfigurationParserException&)
        {
            ZHTTPD_DELETE(config);
            throw;
        }
    }
    else
        throw ConfigurationParserException("A file should have been loaded before parsing");
}

void ConfigurationParser::parseBaseModules(ZHTTPD::Configuration* config)
{
    ticpp::Node* child = 0;
    try
    {
        ModuleParser module_parser;
        ticpp::Node* modulesNode = this->_xml_doc->FirstChildElement()->FirstChildElement("modules");
        while ((child = modulesNode->IterateChildren("module", child)))
            module_parser.parseModule(child, config);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Even if empty, your configuration file MUST contain a 'modules' node");
    }
}

void ConfigurationParser::parseVHosts(ZHTTPD::Configuration* config)
{
    try
    {
        VHostParser vhost_parser;
        vhost_parser.parseVHost(this->_xml_doc->FirstChildElement()->FirstChildElement("vhosts"), config);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Even if empty, your configuration file MUST contain a 'vhosts' node");
    }
}

void ConfigurationParser::parseServer(ZHTTPD::Configuration* config)
{
    ticpp::Node* child = 0;
    try
    {
        ServerParser server_parser;
        ticpp::Node* server_node = this->_xml_doc->FirstChildElement()->FirstChildElement("server");
        while ((child = server_node->IterateChildren(child)))
            server_parser.parseServerConfNode(child, config);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Even if empty, your configuration file MUST contain a 'server' node");
    }
}

