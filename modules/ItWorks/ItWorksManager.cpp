
#include "ItWorksManager.hpp"


ItWorksManager::ItWorksManager() : _name("mod_itworks") {}

// Retourne une nouvelle instance (nous pourrions retourner la même dans ce cas)
ZHTTPD::API::IModule* ItWorksManager::getInstance(bool)
{
    return new ItWorks();
}

// Le serveur redonne une instance quand il n'en a plus besoin
void ItWorksManager::releaseInstance(ZHTTPD::API::IModule* module)
{
    delete module;
}

// Le serveur peut configurer ses modules, ici aucune conf n'est nécessaire
void ItWorksManager::addConfigurationEntry(std::string const&, std::string const&)
{
}

// Notre module est de type PROCESSING
ZHTTPD::API::CATEGORY::Type ItWorksManager::getCategory() const
{
    return ZHTTPD::API::CATEGORY::PROCESSING;
}

// Tout les modules ont un nom
std::string const& ItWorksManager::getName() const
{
    return this->_name;
}

// Utile pour les modules de type filtre (comme gzip),
// Nous renvoyons toujours true
bool ItWorksManager::isRequired(ZHTTPD::API::IRequest const&) const
{
    return true;
}
