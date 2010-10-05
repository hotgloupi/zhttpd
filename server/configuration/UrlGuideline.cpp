
#include "utils/Path.hpp"
#include "utils/Regex.hpp"
#include "utils/Logger.hpp"

#include "UrlGuideline.hpp"

using namespace ZHTTPD;

UrlGuideline::UrlGuideline(std::string const& match_string, bool deny) :
    _match(match_string),
    _deny(deny)
{

}

UrlGuideline::~UrlGuideline()
{

}

bool UrlGuideline::match(API::IRequest& request) const
{
    std::string const& query = request.getRequestQuery();
    API::size_t pos = std::min(query.find_first_of('?'), query.find_first_of('#'));
    bool result = Regex::match(this->_match, query.substr(0, pos));
    LOG_DEBUG("Is '" +query.substr(0, pos)  + "' match '" + this->_match + "' ? " + Logger::toString(result));
    if (this->_deny)
        return !result;
    return result;
}

