
#include "utils/Regex.hpp"
#include "utils/Logger.hpp"
#include "HostGuideline.hpp"

using namespace zhttpd;

HostGuideline::HostGuideline(std::string const& match, bool deny) :
    _match(match),
    _deny(deny)
{

}

HostGuideline::~HostGuideline()
{

}

bool HostGuideline::match(api::IRequest& request) const
{
    std::string const& host = request.getRequestHeader("Host");
    bool result;
    std::size_t pos = host.find(':');
    if (pos != std::string::npos)
        result = Regex::match(this->_match, host.substr(0, pos));
    else
        result = Regex::match(this->_match, host);

    LOG_DEBUG("Is '" + request.getRequestHeader("Host") + "' match '"
              + this->_match + "' ? " + Logger::toString(result));
    if (this->_deny)
        return !result;
    return result;
}

