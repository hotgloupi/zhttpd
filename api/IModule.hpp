
#ifndef __IMODULE_HPP__
# define __IMODULE_HPP__

# include "constants.hpp"
# include "IBuffer.hpp"
# include "IRequest.hpp"

namespace zhttpd
{
    namespace api
    {
        class IModule
        {
        public:
            virtual ~IModule() {}

            /**
             * Cette méthode est le point d'entrée de tout les modules.
             * Retourne vrai si le module s'est occupé de l'évènement.
             * Lorsque cette méthode retourne faux, le serveur doit
             * appliquer le comportement par défaut, en fonction du type
             * d'évènement :
\verbatim
    _________________________________________________________________________
   | event::Type      |       Actions faites sur le serveur                  |
   --------------------------------------------------------------------------|
   | ON_CAN_READ      | Met fin à la requête                                 |
   --------------------------------------------------------------------------|
   |                  | Peut choisir de mettre fin à la requête.             |
   | ON_CAN_WRITE     | Supprime le buffer passé en argument.                |
   --------------------------------------------------------------------------|
   |                  | Le serveur peut arrêter de donner ON_CAN_READ au     |
   | ON_REQUEST_DATA  | module d'entrée et lui donner ON_END.                |
   |                  | Supprime le buffer passé en argument.                |
   --------------------------------------------------------------------------|
   |                  | Le serveur peut arrêter de donner ON_CAN_READ au     |
   | ON_RESPONSE_DATA | module d'entrée et lui donner ON_END.                |
   |                  | Supprime le buffer passé en argument.                |
   --------------------------------------------------------------------------|
   | ON_IDLE          | Donne ON_END à ce même module.                       |
   --------------------------------------------------------------------------|
   | ON_END           | Redonne ON_END au prochain module.                   |
   --------------------------------------------------------------------------|
   | ON_ERROR         | Redonne ON_ERROR au prochain module.                 |
   |_________________________________________________________________________|

\endverbatim
             * @param event Type de l'évènement
             * @param request La requête
             * @param buffer Un pointeur vers le buffer à traiter
             */
            virtual bool processRequest(event::Type event,
                                        IRequest* request,
                                        IBuffer* buffer) = 0;
        };
    }
}

#endif /* !__IMODULE_HPP__ */

