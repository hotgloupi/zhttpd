#ifndef URLREWRITE_HPP
#define URLREWRITE_HPP

# include "constants.hpp"
# include "IBuffer.hpp"
# include "IRequest.hpp"
# include "IModule.hpp"

using namespace ZHTTPD::API;

namespace ZHTTPD
{
    namespace Module
    {
        class UrlRewrite : IModule
        {
        public:

            /**
             * Cette méthode est le point d'entrée de tout les modules.
             * @param event Type de l'évènement
             * @param request La requête
             * @param buffer Un pointeur vers le buffer à traiter (possiblement 0)
             */
            void processRequest(EVENT::Type event, IRequest* request, IBuffer* buffer);

            UrlRewrite();
            ~UrlRewrite();
        };
    }
}

#endif // URLREWRITE_HPP
