
#ifndef __IMODULEMANAGER_HPP__
# define __IMODULEMANAGER_HPP__

# include <string>

# include "IModule.hpp"

namespace ZHTTPD
{
    namespace API
    {
        class IModuleManager
        {
        public:
            virtual ~IModuleManager() {}

            /**
             * Retourne un nouveau module
             * @param in_response Le module demandé sera dans la chaîne de réponse
             *                    (après le module de catégory PROCESSING) si
             *                    `in_response` vaut vrai.
             */
            virtual IModule* getInstance(bool in_response = false) = 0;

            /**
             * Signale au getionnaire que cette instance n'est plus utilisée
             */
            virtual void releaseInstance(IModule* module) = 0;

            /**
             * Permet de configurer le module (clef = valeur)
             */
            virtual void addConfigurationEntry(std::string const& key,
                                               std::string const& value) = 0;

            /**
             * Informe sur la catégorie à laquelle ce module appartient
             */
            virtual CATEGORY::Type getCategory() const = 0;

            /**
             * Retourne le nom du module.
             */
            virtual std::string const& getName() const = 0;

            /**
             * Permet de savoir si le module est requis pour une
             * requête donnée.
             */
            virtual bool isRequired(IRequest const& request) const = 0;
        };
    }
}

#endif /* !__IMODULEMANAGER_HPP__ */

