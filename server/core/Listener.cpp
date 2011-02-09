
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <stdexcept>

#include "utils/macros.hpp"
#include "utils/Logger.hpp"

#include "core/Session.hpp"
#include "core/Listener.hpp"

using namespace zhttpd;

Listener::Listener(callback_t const& on_new_connection, obtain_service_t const& get_io_service) :
    _acceptors(),
    _on_new_connection(on_new_connection),
    _get_io_service(get_io_service),
    _is_running(false),
    _stopped(true)
{
}

Listener::~Listener()
{
    std::list<acceptor_t*>::iterator it = this->_acceptors.begin(),
                                     end = this->_acceptors.end();
    for (; it != end; ++it)
    {
        (*it)->cancel();
        (*it)->close();
    }
    this->_acceptors.clear();
}

void Listener::bind(Listener::address_t const& address, api::uint16_t port)
{
    try
    {
        acceptor_ptr_t acceptor(new Listener::acceptor_t(*this));
        Listener::endpoint_t endpoint(address, port);
        acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor->open(endpoint.protocol());
        acceptor->bind(endpoint);
        acceptor->listen();
        this->_acceptors.push_back(acceptor);
        this->_connectAcceptor(acceptor);
    }
    catch (std::exception& err)
    {
        LOG_ERROR("Cannot bind " + Logger::toString(port) + ": " + err.what());
    }
}

void Listener::_connectAcceptor(Listener::acceptor_ptr_t acceptor)
{
    Listener::socket_ptr_t new_connection(
        new Listener::socket_t(this->_get_io_service(), acceptor->local_endpoint())
    );
    acceptor.async_accept(
        *new_connection,
        boost::bind(&Listener::_onNewConnection, this, acceptor, new_connection, boost::asio::placeholders::error)
    );
}

void Listener::_onNewConnection(Listener::acceptor_ptr_t acceptor,
                                Session::socket_ptr_t socket,
                                boost::system::error_code const& e)
{
    this->_on_new_connection(socket, acceptor->local_endpoint().port());
    this->_connectAcceptor(acceptor);
}

