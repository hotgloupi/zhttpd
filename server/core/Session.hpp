
#ifndef __SESSION_HPP__
# define __SESSION_HPP__

# include "socket/Socket.hpp"
# include "api/ISession.hpp"

namespace zhttpd
{
    class Session : public api::ISession
    {
    private:
        Socket* _socket;
        mutable api::uint32_t _ip;
        api::uint16_t _port;
        api::size_t _processed_request_count;
        api::socket_t _fd;

    public:
        Session(Socket* socket, api::uint16_t port);
        ~Session();
        api::socket_t getSocket() const;
        api::uint32_t getIp() const;
        api::uint16_t getPort() const;
        void incrProcessedRequestCount();
        api::size_t getProcessedRequestCount() const;

        // méthodes non présentes dans l'API
        Socket& getServerSocket();
        Socket* getServerSocketPtr();
        Socket& popServerSocket();
    };
}

#endif /* !__SESSION_HPP__ */

