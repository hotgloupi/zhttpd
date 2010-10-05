
#ifndef __ISOCKETEVENTHANDLER_HPP__
# define __ISOCKETEVENTHANDLER_HPP__

namespace ZHTTPD
{
    namespace SOCKET_EVENT
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
        virtual void handleSocketEvent(SOCKET_EVENT::Type evt) = 0;
    };
}

#endif /* !__ISOCKETEVENTHANDLER_HPP__ */

