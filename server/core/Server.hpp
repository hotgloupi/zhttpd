/**
 * @file Server.hpp
 * @author <londei_a@epitech.net> Raphaël Londeix
 *
 * @section DESCRIPTION
 * @FIXME@
 */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <string>
# include <boost/noncopyable.hpp>

# include "core/SessionManager.hpp"

namespace boost { namespace asio { class io_service; } }

namespace zhttpd
{
    class Session;
    class Listener;
    class Rcon;
    class Configuration;

    class Server : private boost::noncopyable
    {
    private:
        SessionManager<>*           _session_manager;
        boost::asio::io_service*    _io_service;
        Listener*                   _listener;
        Rcon*                       _rcon;
        std::string                 _configuration_path;

   public:
        Server();
        ~Server();
        int run(std::string const& configuration_path);
        void stop();
        bool reload();

    private:
        void _loadListener(Configuration* config);
        void _onNewConnection(std::auto_ptr<Listener::socket_t>& socket, api::uint16_t port);
        boost::asio::io_service& _getIOService();
    };
}

#endif /* !__SERVER_HPP__ */

