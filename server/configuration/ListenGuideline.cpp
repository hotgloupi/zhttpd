
#include "utils/Regex.hpp"
#include "ListenGuideline.hpp"

using namespace zhttpd;

ListenGuideline::ListenGuideline(api::uint16_t port, bool deny) :
    _port(port),
    _deny(deny)
{

}

ListenGuideline::~ListenGuideline()
{

}

bool ListenGuideline::match(api::IRequest& request) const
{
    if (request.getSession().getPort() == this->_port && this->_deny)
        return false;
    return true;
}

