
#ifndef __SESSION_HPP__
# define __SESSION_HPP__

# include "socket/Socket.hpp"
# include "api/ISession.hpp"

namespace ZHTTPD
{
    class Session : public API::ISession
    {
    private:
        Socket* _socket;
        mutable API::uint32_t _ip;
        API::uint16_t _port;
        API::size_t _processed_request_count;
        API::socket_t _fd;

    public:
        Session(Socket* socket, API::uint16_t port);
        ~Session();
        API::socket_t getSocket() const;
        API::uint32_t getIp() const;
        API::uint16_t getPort() const;
        void incrProcessedRequestCount();
        API::size_t getProcessedRequestCount() const;

        // méthodes non présentes dans l'API
        Socket& getServerSocket();
        Socket* getServerSocketPtr();
        Socket& popServerSocket();
    };
}

#endif /* !__SESSION_HPP__ */

