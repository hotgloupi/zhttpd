
#include "utils/macros.hpp"
#include "ModuleParser.hpp"
#include "ConfigurationParserException.hpp"

using namespace zhttpd;
using namespace zhttpd::parser;

ModuleParser::ModuleParser()
{
}

ModuleParser::~ModuleParser()
{
}

void ModuleParser::parseModule(ticpp::Node* node, Configuration* config)
{
    ticpp::Node* child = 0;
    try
    {
        std::string name;
        node->ToElement()->GetAttribute("name", &name);

        std::string enabledStr = node->ToElement()->GetAttribute("enabled");
        bool enabled = true;
        if (enabledStr == "false")
            enabled = false;
        // if (enabled)
        // {
            ModuleConfiguration* module = new ModuleConfiguration(name, enabled, *config);
            config->addModuleConfiguration(*module);
            while ((child = node->IterateChildren("option", child)))
                this->parseOption(child, module);
        // }
    }
    catch (ticpp::Exception& e)
    {
        throw ConfigurationParserException("module misformed. Maybe you forgot the name attribute");
    }
    catch (ConfigurationParserException& e)
    {
        throw;
    }
}

void ModuleParser::parseModule(ticpp::Node* node, VHost* vhost, Configuration* config)
{
    ticpp::Node* child = 0;
    try
    {
        std::string name;
        node->ToElement()->GetAttribute("name", &name);

        std::string enabledStr = node->ToElement()->GetAttribute("enabled");
        bool enabled = true;
        if (enabledStr == "false")
            enabled = false;
        //if (enabled)
        //{
            ModuleConfiguration* module = new ModuleConfiguration(name, enabled, *config);
            vhost->addModule(module);

            while ((child = node->IterateChildren("option", child)))
                this->parseOption(child, module);
        //}
    }
    catch (ticpp::Exception& e)
    {
        throw ConfigurationParserException("Module misformed. Maybe you forgot the name attribute");
    }
    catch (ConfigurationParserException& e)
    {
        throw;
    }
}

void ModuleParser::parseOption(ticpp::Node* node, ModuleConfiguration* module)
{
    try
    {
        std::string key;
        std::string value;

        ticpp::Element* e = node->ToElement();
        e->GetAttribute("key", &key);
        e->GetAttribute("value", &value);

        module->addConfigurationEntry(key, value);
    }
    catch (ticpp::Exception& e)
    {
        throw ConfigurationParserException("Module option misformed. It should contain key and value attributes");
    }
}

