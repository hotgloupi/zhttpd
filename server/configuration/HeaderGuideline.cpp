
#include "utils/Regex.hpp"
#include "HeaderGuideline.hpp"

using namespace ZHTTPD;

HeaderGuideline::HeaderGuideline(std::string const& key, std::string const& match, bool deny) :
    _key(key),
    _match(match),
    _deny(deny)
{

}

HeaderGuideline::~HeaderGuideline()
{

}

bool HeaderGuideline::match(API::IRequest& request) const
{
    bool result = Regex::match(this->_match, request.getRequestHeader(this->_key));
    if (this->_deny)
        return !result;
    return result;
}
