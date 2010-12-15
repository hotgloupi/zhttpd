
#include "ItWorksManager.hpp"


ItWorksManager::ItWorksManager() : _name("mod_itworks") {}

// Retourne une nouvelle instance (nous pourrions retourner la même dans ce cas)
zhttpd::api::IModule* ItWorksManager::getInstance(bool)
{
    return new ItWorks();
}

// Le serveur redonne une instance quand il n'en a plus besoin
void ItWorksManager::releaseInstance(zhttpd::api::IModule* module)
{
    delete module;
}

// Le serveur peut configurer ses modules, ici aucune conf n'est nécessaire
void ItWorksManager::addConfigurationEntry(std::string const&, std::string const&)
{
}

// Notre module est de type PROCESSING
zhttpd::api::category::Type ItWorksManager::getCategory() const
{
    return zhttpd::api::category::PROCESSING;
}

// Tout les modules ont un nom
std::string const& ItWorksManager::getName() const
{
    return this->_name;
}

// Utile pour les modules de type filtre (comme gzip),
// Nous renvoyons toujours true
bool ItWorksManager::isRequired(zhttpd::api::IRequest const&) const
{
    return true;
}
