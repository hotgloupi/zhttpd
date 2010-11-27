
#ifndef __ISOCKETEVENTHANDLER_HPP__
# define __ISOCKETEVENTHANDLER_HPP__

namespace zhttpd
{
    namespace socket_event
    {
        enum Type
        {
            CAN_READ = 0,
            CAN_WRITE = 1,
            HAS_ERROR = 2,
            NB_SOCKET_EVENT = 3,
        };
    }

    class ISocketEventHandler
    {
    public:
        virtual ~ISocketEventHandler() {}
        virtual void handleSocketEvent(socket_event::Type evt) = 0;
    };
}

#endif /* !__ISOCKETEVENTHANDLER_HPP__ */

