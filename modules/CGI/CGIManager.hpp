#ifndef CGIMANAGER_HPP
# define CGIMANAGER_HPP

# include "server/modules/common/AbstractManager.hpp"
# include "api/constants.hpp"
# include "api/IModule.hpp"
# include "api/IModuleManager.hpp"
# include "CGI.hpp"

typedef ZHTTPD::MOD::StatefullManager<CGI, ZHTTPD::MOD::POLICIES::MapConfigurationPolicy> ConfigurableStatefullManager;

class CGIManager : public ConfigurableStatefullManager
{
public:
    CGIManager() :
       ConfigurableStatefullManager("mod_cgi", ZHTTPD::API::CATEGORY::PROCESSING)
    {
    }

    virtual ~CGIManager() {}

    virtual bool isRequired(ZHTTPD::API::IRequest const& request) const
    {
        std::string::size_type idx = request.getFilePath().rfind('.');
        if(idx == std::string::npos)
            return false;
        return (this->_configuration.find(request.getFilePath().substr(idx + 1)) != this->_configuration.end());
    }
};

#endif // CGIMANAGER_HPP
