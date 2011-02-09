
#ifndef __LISTENER_HPP__
# define __LISTENER_HPP__

# include <list>
# include <boost/asio/ip/address.hpp>
# include <boost/asio/ip/tcp.hpp>
# include <boost/asio/io_service.hpp>
# include <boost/system/error_code.hpp>

# include <boost/noncopyable.hpp>
# include <boost/function.hpp>
# include <boost/shared_ptr.hpp>

# include "api/types.hpp"

namespace zhttpd
{
    class Session;

    /**
     * Listening for new connections and call given callback
     */
    template<typename Allocator>
    class Listener : private boost::noncopyable
    {
    public:
        typedef boost::asio::ip::address address_t;
        typedef boost::asio::ip::tcp::endpoint endpoint_t;
        typedef boost::asio::ip::tcp::acceptor acceptor_t;
        typedef boost::asio::ip::tcp::socket socket_t;
        typedef boost::function<void(std::auto_ptr<socket_t>, api::uint16_t)> callback_t;
        typedef boost::function<boost::asio::io_service&(void)> obtain_service_t;

    private:
        boost::asio::io_service     _io_service;
        std::list<acceptor_t*>      _acceptors;
        callback_t                  _on_new_connection;
        obtain_service_t            _get_io_service;
        volatile bool               _is_running;
        volatile bool               _stopped;

    public:
        Listener(callback_t const& on_new_connection, obtain_service_t const& get_io_service);
        ~Listener();

        /**
         * Bind an address and a port, callable multiple times
         */
        void bind(address_t const& address, api::uint16_t port);
        void run();
        void stop();

    private:
        void _onNewConnection(acceptor_t& acceptor,
                              boost::shared_ptr<socket_t> socket,
                              boost::system::error_code const& e);
        void _connectAcceptor(acceptor_t& acceptor);
    };
}

#endif /* !__LISTENER_HPP__ */

