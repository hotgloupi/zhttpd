
#ifndef __ISESSION_HPP__
# define __ISESSION_HPP__

# include "types.hpp"

namespace ZHTTPD
{
    namespace API
    {
        class ISession
        {
        public:
            virtual ~ISession() {}

            /**
             * Retourne le file descriptor natif.
             */
            virtual socket_t getSocket() const = 0;

            /**
             * Retourne l'IP du client.
             */
            virtual uint32_t getIp() const = 0;

            /**
             * Retourne le port du client.
             */
            virtual uint16_t getPort() const = 0;

            /**
             * Retourne le nombre de requêtes executées avec cette session
             */
            virtual size_t getProcessedRequestCount() const = 0;
        };
    }
}

#endif /* !__ISESSION_HPP__ */

