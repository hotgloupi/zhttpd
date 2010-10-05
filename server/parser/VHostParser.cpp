
#include "configuration/UrlGuideline.hpp"
#include "configuration/HeaderGuideline.hpp"
#include "configuration/HostGuideline.hpp"
#include "configuration/ListenGuideline.hpp"

#include "VHostParser.hpp"
#include "ConfigurationParserException.hpp"
#include "ModuleParser.hpp"

using namespace ZHTTPD;
using namespace ZHTTPD::PARSER;

VHostParser::VHostParser()
{
    this->_hdlrs["url"] = &VHostParser::_parseUrl;
    this->_hdlrs["document-root"] = &VHostParser::_parseDocumentRoot;
    this->_hdlrs["header"] = &VHostParser::_parseHeader;
    this->_hdlrs["host"] = &VHostParser::_parseHost;
    this->_hdlrs["listen"] = &VHostParser::_parseListen;
}

VHostParser::~VHostParser()
{
}

void VHostParser::parseVHost(ticpp::Node* node, Configuration* conf, VHost* parent)
{
    try
    {
        ModuleParser module_parser;
        VHost* vhost = new VHost(parent, *conf);
        vhost->setName(node->ToElement()->GetAttribute("name"));
        ticpp::Node* child = 0;
        std::string childName;
        while ((child = node->IterateChildren(child)))
        {
            childName = child->ToElement()->Value();
            if (childName == "vhost")
                this->parseVHost(child, conf, vhost);
            else if (childName == "module")
                module_parser.parseModule(child, vhost, conf);
            else
                this->parseGuideLine(child, vhost);
        }
        if (parent == 0)
            conf->setVHost(*vhost);
        else
            parent->addChild(vhost);
    }
    catch (ticpp::Exception& e)
    {
        throw ConfigurationParserException(std::string("Undefined error while parsing VHosts: ") + e.what());
    }
}

void VHostParser::parseGuideLine(ticpp::Node* node, VHost* vhost)
{
    try
    {
        std::string node_name = node->ToElement()->Value();

        if (this->_hdlrs.find(node_name) != this->_hdlrs.end())
            (this->*this->_hdlrs[node_name])(node, vhost);
        else
            std::cout << node_name << " non gere: skip" << std::endl;
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("A virtual host guideline shoud be a valid XML tag");
    }
}

void VHostParser::_parseDocumentRoot(ticpp::Node* node, VHost* vhost)
{
    try
    {
        std::string path;
        node->ToElement()->GetAttribute("path", &path);
        vhost->setDocumentRoot(path);
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("Document-root guideline should have a 'path' attribute");
    }
}

void VHostParser::_parseUrl(ticpp::Node* node, VHost* vhost)
{
    try
    {
        ticpp::Element* urlElement = node->ToElement();
        std::string match;
        urlElement->GetAttribute("match", &match);
        std::string denyStr = urlElement->GetAttribute("deny");
        bool deny = false;
        if (denyStr == "true")
            deny = true;
        vhost->addGuideline(new UrlGuideline(match, deny));
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("URL guideline misformed. It should contain at least a 'match' attribute, and may have a 'deny' one");
    }
}

void VHostParser::_parseHeader(ticpp::Node* node, VHost* vhost)
{
    try
    {
        ticpp::Element* headerElement = node->ToElement();
        std::string key;
        std::string match;
        headerElement->GetAttribute("key", &key);
        headerElement->GetAttribute("match", &match);
        std::string denyStr = headerElement->GetAttribute("deny");
        bool deny = false;
        if (denyStr == "true")
            deny = true;
        vhost->addGuideline(new HeaderGuideline(key, match, deny));
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("header guideline misformed. It should contain 'key' and 'match' attributes, and may have a 'deny' one");
    }
}

void VHostParser::_parseHost(ticpp::Node* node, VHost* vhost)
{
    try
    {
        std::string match;
        node->ToElement()->GetAttribute("match", &match);
        std::string denyStr = node->ToElement()->GetAttribute("deny");
        bool deny = false;
        if (denyStr == "true")
            deny = true;
        vhost->addGuideline(new HostGuideline(match, deny));
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("host guideline misformed. It should have a 'match' attribute, and mey have a deny");
    }
}

void VHostParser::_parseListen(ticpp::Node* node, VHost* vhost)
{
    try
    {
        ZHTTPD::API::uint16_t port;
        node->ToElement()->GetAttribute("port", &port);
        std::string denyStr = node->ToElement()->GetAttribute("deny");
        bool deny = false;
        if (denyStr == "true")
            deny = true;
        vhost->addGuideline(new ListenGuideline(port, deny));
    }
    catch (ticpp::Exception&)
    {
        throw ConfigurationParserException("listen guideline misformed. It should have a 'port' attribute, containing an integer value, and may have a deny attribute");
    }
}

