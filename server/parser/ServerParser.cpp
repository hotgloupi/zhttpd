
#include "api/types.hpp"
#include "configuration/Configuration.hpp"

#include "ServerParser.hpp"
#include "ConfigurationParserException.hpp"

using namespace ZHTTPD;
using namespace ZHTTPD::PARSER;

ServerParser::ServerParser()
{
    this->_confParsers["listen"] = &ServerParser::_parseListen;
    this->_confParsers["modules-directory"] = &ServerParser::_parseModuleDirectory;
    this->_confParsers["index-files"] = &ServerParser::_parseIndexFiles;
}

ServerParser::~ServerParser()
{

}

void ServerParser::parseServerConfNode(ticpp::Node* node, Configuration* conf)
{
    std::string name = node->ToElement()->Value();
    if (this->_confParsers.find(name) != this->_confParsers.end())
    {
        (this->*this->_confParsers[name])(node, conf);
        return;
    }
    throw ConfigurationParserException("Invalid configuration for server: unknown option " + name);
}

void ServerParser::_parseListen(ticpp::Node* node, Configuration* conf)
{
    try
    {
        API::uint16_t port;
        node->ToElement()->GetAttribute("port", &port);
        std::string iomodule = node->ToElement()->GetAttribute("io-module");
        conf->addListenPort(port, iomodule);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("listen option misformed. Check if there is an integer 'port' attribute");
    }
}

void ServerParser::_parseModuleDirectory(ticpp::Node* node, Configuration* conf)
{
    try
    {
        std::string path;
        node->ToElement()->GetAttribute("path", &path);
        conf->addModuleDirectory(path);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("modules directory option misformed. It should have a 'path' attribute");
    }
}

void ServerParser::_parseIndexFiles(ticpp::Node* node, Configuration* conf)
{
    try
    {
        ticpp::Node* child = 0;
        while ((child = node->IterateChildren("filename", child)))
            this->_parseIndexFileName(child, conf);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Index files configuration misformed");
    }
    catch (ConfigurationParserException&)
    {
        throw ;
    }
}

void ServerParser::_parseIndexFileName(ticpp::Node* node, Configuration* conf)
{
    try
    {
        std::string match;
        node->ToElement()->GetAttribute("match", &match);
        conf->addIndexFile(match);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("filename misformed in index files configuration. It should contain a 'match' attribute");
    }
}

