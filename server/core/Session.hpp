
#ifndef __SESSION_HPP__
# define __SESSION_HPP__

# include <boost/asio/ip/tcp.hpp>
# include <boost/shared_ptr.hpp>

# include "api/ISession.hpp"

namespace zhttpd
{
    class Session : public api::ISession
    {
    public:
        typedef boost::asio::ip::tcp::socket socket_t;
    private:
        socket_t* _socket;
        api::uint16_t _port;
        api::size_t _processed_request_count;
        api::socket_t _fd;

    public:
        Session(std::auto_ptr<socket_t> socket, api::uint16_t port);
        virtual ~Session();
        api::socket_t getSocket() const;
        api::uint32_t getIp() const;
        api::uint16_t getPort() const;
        void incrProcessedRequestCount();
        api::size_t getProcessedRequestCount() const;
    };
}

#endif /* !__SESSION_HPP__ */

