
#ifndef __ISOCKETEVENTNOTIFIER_HPP__
# define __ISOCKETEVENTNOTIFIER_HPP__

# include "api/types.hpp"

namespace zhttpd
{
    class ISocketEventNotifier
    {
    public:
        virtual ~ISocketEventNotifier() {}
        virtual void notify(api::socket_t fd, socket_event::Type evt) = 0;
    };
}

#endif /* !__ISOCKETEVENTNOTIFIER_HPP__ */

