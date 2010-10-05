
#ifndef __ISOCKETEVENTNOTIFIER_HPP__
# define __ISOCKETEVENTNOTIFIER_HPP__

# include "api/types.hpp"

namespace ZHTTPD
{
    class ISocketEventNotifier
    {
    public:
        virtual ~ISocketEventNotifier() {}
        virtual void notify(API::socket_t fd, SOCKET_EVENT::Type evt) = 0;
    };
}

#endif /* !__ISOCKETEVENTNOTIFIER_HPP__ */

