
#ifndef __IBUFFER_HPP__
# define __IBUFFER_HPP__

# include "types.hpp"

namespace ZHTTPD
{
    namespace API
    {
        class IBuffer
        {
        public:
            virtual ~IBuffer() {}

            /**
             * Retourne les données brutes. Le pointeur n'est jamais NULL.
             */
            virtual char* getRawData() = 0;

            /**
             * Retourne la taille du buffer (toujours > 0).
             */
            virtual API::size_t getSize() const = 0;

            /**
             * Redimensionne le buffer à la taille spécifiée.
             * Les données présentes sont conservées (fonctionne comme realloc)
             * @param size Taille totale voulue en octet.
             */
            virtual void setSize(API::size_t size) = 0;

            /**
             * Redimensionne le buffer à la taille spécifiée.
             * Les données comprisent entre l'offset `start` et `start + old_size` sont conservées
             * et le nouveau buffer commence à l'offset `start`.
             * @param start Offset de départ.
             * @param size Taille totale voulue en octet.
             * \exemple
             *  b = buffer_manager.allocate("abcdef");
             *  b.setSize(2, 4); // b contient "cdef"
             */
            virtual void setSize(API::size_t start, API::size_t size) = 0;

        };
    }
}

#endif /* !__IBUFFER_HPP__ */

