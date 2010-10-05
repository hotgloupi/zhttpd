
#ifndef __IBUFFERMANAGER_HPP__
# define __IBUFFERMANAGER_HPP__

# include <string>

# include "IBuffer.hpp"

namespace ZHTTPD
{
    namespace API
    {
        class IBufferManager
        {
        public:
            virtual ~IBufferManager() {}

            /**
             * Création d'un nouveau buffer à partir d'une
             * chaîne de caractères.
             */
            virtual IBuffer* allocate(std::string const& string) = 0;

            /**
             * Création d'un nouveau buffer à partir d'un
             * tableau de données brutes.
             */
            virtual IBuffer* allocate(char const* data, size_t size) = 0;

            /**
             * Création d'un nouveau buffer de la taille spécifiée.
             */
            virtual IBuffer* allocate(size_t size) = 0;

            /**
             * Signale au gestionnaire que ce buffer ne sera plus utilisé.
             */
            virtual void release(IBuffer* buffer) = 0;
        };
    }
}

#endif /* !__IBUFFERMANAGER_HPP__ */

