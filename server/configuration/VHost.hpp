
#ifndef __VHOST_HPP__
# define __VHOST_HPP__

# include <map>
# include <string>

# include "api/IRequest.hpp"

# include "ModuleConfiguration.hpp"
# include "IVHostGuideline.hpp"

namespace ZHTTPD
{
    class Configuration;

    class VHost
    {
    private:
        Configuration&                              _configuration;
        std::list<VHost*>                           _childs;
        VHost*                                      _parent;
        std::string                                 _name;
        std::string                                 _document_root;
        std::map<std::string, ModuleConfiguration*> _modules;
        std::list<IVHostGuideline*>                 _guide_lines;

    public:
        VHost(VHost* parent, Configuration& conf);
        virtual ~VHost();
        VHost* match(API::IRequest& request);

        void addModule(ModuleConfiguration* module);
        ZHTTPD::API::IModuleManager* getModule(std::string const& name);
        ModuleConfiguration& getModuleConfiguration(std::string const& modname);

        bool moduleExists(std::string const& name);
        void addGuideline(IVHostGuideline* guideLine);
        void addChild(VHost* vhost);
        void setDocumentRoot(std::string const& path);
        std::string const& getDocumentRoot();
        std::string const& getName() const;
        void setName(std::string const& name);
    };
}

#endif // VHOST_HPP__

