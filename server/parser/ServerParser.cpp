
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "api/types.hpp"
#include "configuration/Configuration.hpp"
#include "utils/Logger.hpp"

#include "ServerParser.hpp"
#include "ConfigurationParserException.hpp"

using namespace zhttpd;
using namespace zhttpd::parser;

ServerParser::ServerParser()
{
    this->_confParsers["listen"] = &ServerParser::_parseListen;
    this->_confParsers["modules-directory"] = &ServerParser::_parseModuleDirectory;
    this->_confParsers["index-files"] = &ServerParser::_parseIndexFiles;
    this->_confParsers["log"] = &ServerParser::_parseLogFile;
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
        api::uint16_t port;
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

void ServerParser::_parseLogFile(ticpp::Node* node, Configuration*)
{
    std::string match;
    try
    {
        node->ToElement()->GetAttribute("file", &match);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Wrong file in log configuration. It should contain a 'file' attribute");
    }
    try
    {
        std::ofstream* o = new std::ofstream(match.c_str(), std::ios_base::out | std::ios_base::app);
        if (o->fail())
        {
            delete o;
            throw std::runtime_error("Cannot open log file '" + match + "'");
        }
        Logger::getInstance()->setOutput(*o);
    }
    catch (std::exception& e)
    {
        throw ConfigurationParserException("Cannot set log file: " + Logger::toString(e.what()));
    }
}
