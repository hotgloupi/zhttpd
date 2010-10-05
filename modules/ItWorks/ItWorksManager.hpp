
#ifndef __ITWORKSMANAGER_HPP__
# define __ITWORKSMANAGER_HPP__

#include <string>

# include "api/IModuleManager.hpp"
# include "api/constants.hpp"

# include "ItWorks.hpp"

class ItWorksManager : public ZHTTPD::API::IModuleManager
{
private:
    std::string _name;

public:
    ItWorksManager();

// Retourne une nouvelle instance (nous pourrions retourner la même dans ce cas)
    virtual ZHTTPD::API::IModule* getInstance(bool);

// Le serveur redonne une instance quand il n'en a plus besoin
    virtual void releaseInstance(ZHTTPD::API::IModule* module);

// Le serveur peut configurer ses modules, ici aucune conf n'est nécessaire
    virtual void addConfigurationEntry(std::string const&, std::string const&);

// Notre module est de type PROCESSING
    virtual ZHTTPD::API::CATEGORY::Type getCategory() const;

// Tout les modules ont un nom
    virtual std::string const& getName() const;

// Utile pour les modules de type filtre (comme gzip),
// Nous renvoyons toujours true
    virtual bool isRequired(ZHTTPD::API::IRequest const& request) const;
};

#endif /* !__ITWORKSMANAGER_HPP__ */

