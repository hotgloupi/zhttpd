
#ifndef __IREQUEST_HPP__
# define __IREQUEST_HPP__

# include <list>
# include <string>

# include "constants.hpp"
# include "IBuffer.hpp"
# include "IBufferManager.hpp"
# include "ISession.hpp"

namespace ZHTTPD
{
    namespace API
    {
        class IRequest
        {
        public:
            virtual ~IRequest() {}

            /*****************************************************************
             * Partie serveur:                                               *
             *      Dans cette partie, les méthodes signalent au serveur     *
             *      plusieurs types d'évènements, matérialisés par ces       *
             *      méthodes.                                                *
             *****************************************************************/

            /**
             * Demande au serveur d'être rappelé plus tard. Le nombre de
             * de millisecondes est optionnel et le module n'a aucune garantie
             * d'être rappelé après le temps fournit en argument. Le serveur
             * peut l'utiliser pour minimiser le nombre d'appel aux modules.
             */
            virtual void callLater(uint32_t ms = 0) = 0;

            /**
             * Le module donne au module suivant le buffer en argument.
             * Celui-ci sera supprimé par le dernier module à l'utiliser.
             */
            virtual void giveData(IBuffer* buffer) = 0;

            /**
             * Le module signale au serveur qu'une erreur s'est produite.
             * Si la chaîne n'est pas fournie, le serveur devra placer le
             * message par défaut.
             */
            virtual void raiseError(HTTP_CODE::Type code, std::string error = "") = 0;

            /**
             * Le module signale que la requête est terminée. Le serveur devra
             * supprimer la requête dès que possible. Tous les modules suivant vont
             * donc recevoir dans l'ordre le signal EVENT::ON_END.
             */
            virtual void raiseEnd() = 0;

            /**
             * Cette méthode demande au serveur de rappeler ce module
             * lorsqu'il est possible d'écrire sur la socket.
             */
            virtual void needWrite(IBuffer* buffer) = 0;

            /**
             * Récupère la session liée à cette requête.
             */
            virtual ISession const& getSession() = 0;

            /**
             * Demande le gestionnaire de buffer au serveur.
             */
            virtual IBufferManager& getBufferManager() = 0;



            /*****************************************************************
             * Méthodes pour la partie requête :                             *
             *****************************************************************/

            /**
             * Retourne le fichier ciblé par la requête. C'est le chemin absolu
             * vers le fichier, déterminé par le serveur.
             */
            virtual std::string const& getFilePath() const = 0;

            /**
             * Ajoute ou modifie un header de la requête
             */
            virtual void setRequestHeader(std::string const& key, std::string const& val) = 0;

            /**
             * Retourne la valeur d'un header de la requête, ou une
             * chaîne vide si le header n'existe pas
             */
            virtual std::string const& getRequestHeader(std::string const& key) const = 0;

            /**
             * Retourne toutes les clefs des headers présents dans la requête
             */
            virtual std::list<std::string const*> getRequestHeaderKeys() const = 0;

            /**
             * Affecte la méthode HTTP à la requête.
             */
            virtual void setRequestMethod(HTTP_METHOD::Type method) = 0;

            /**
             * Retourne la méthode HTTP de la requête.
             */
            virtual HTTP_METHOD::Type getRequestMethod() const = 0;

            /**
             * Affecte l'argument de la méthode HTTP (la query) à
             * la requête.
             */
            virtual void setRequestQuery(std::string const& query) = 0;

            /**
             * Récupère l'argument de la méthode HTTP (la query) à
             * la requête.
             */
            virtual std::string const& getRequestQuery() const = 0;

            /*****************************************************************
             * Méthodes pour la partie réponse :                             *
             *****************************************************************/

            /**
             * Affecte le code de retour pour la réponse HTTP.
             */
            virtual void setResponseCode(HTTP_CODE::Type code, std::string const& reason = "") = 0;

            /**
             * Récupère le code de retour pour la réponse HTTP. S'il n'est pas défini,
             * retourne la valeur ZHTTPD::API::HTTP_CODE::UNDEFINED
             */
            virtual HTTP_CODE::Type getResponseCode() const = 0;


            /**
             * Récupère le message associé au code d'erreur
             */
            virtual std::string const& getResponseMessage() const = 0;

            /**
             * Ajoute ou modifie une entête pour la réponse (clef: valeur)
             */
            virtual void setResponseHeader(std::string const& key, std::string const& val) = 0;

            /**
             * Récupère un header de réponse par sa clef. Si celui-ci n'est pas présent, alors
             * une chaîne vide est retournée.
             */
            virtual std::string const& getResponseHeader(std::string const& key) const = 0;

            /**
             * Récupère la liste de toutes les clefs des entêtes présentes.
             */
            virtual std::list<std::string const*> getResponseHeaderKeys() const = 0;

        };
    }
}
#endif /* !__IREQUEST_HPP__ */

