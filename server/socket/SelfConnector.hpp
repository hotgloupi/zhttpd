
#ifndef __SELFCONNECTOR_HPP__
# define __SELFCONNECTOR_HPP__

#include "socket/Socket.hpp"
#include "api/types.hpp"
#include "thread/ITask.hpp"
#include "thread/Mutex.hpp"

namespace zhttpd
{
    class SelfConnector : public ITask
    {
    private:
        api::uint16_t _port;
        Socket* _socket;
        Mutex _mutex;

    public:
        SelfConnector(api::uint16_t port);
        ~SelfConnector();
        void run();
        Socket* getSocket();
    };
}

#endif // !__SELFCONNECTOR_HPP__
