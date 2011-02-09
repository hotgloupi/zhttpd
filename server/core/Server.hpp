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
# include <boost/shared_ptr.hpp>

# include "core/SessionManager.hpp"

# ifdef ZHTTPD_DEBUG
#  include "utils/SafeAllocator.hpp"
#  define ZHTTPD_DEFAULT_ALLOCATOR zhttpd::SafeAllocator
# else
#  include "utils/LockedAllocator.hpp"
#  define ZHTTPD_DEFAULT_ALLOCATOR zhttpd::LockedAllocator
# endif

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
        typedef SessionManager<ZHTTPD_DEFAULT_ALLOCATOR> SessionManagerType;

    private:
        SessionManagerType*         _session_manager;
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
        void _onNewConnection(boost::shared_ptr<Session::socket_t> socket, api::uint16_t port);
        boost::asio::io_service& _getIOService();
    };
}

#endif /* !__SERVER_HPP__ */

