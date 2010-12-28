
# include "Response.hpp"

using namespace types;

void Response::addField(std::string const& key, std::string const& val)
{
    this->_fields.push_back(pair_t(key, val));
}

Response::fields_t const& Response::getFields() const
{
    return this->_fields;
}

void Response::addDetail(std::string const& key, std::string const& val)
{
    this->_details.push_back(pair_t(key, val));
}

Response::fields_t const& Response::getDetails() const
{
    return this->_details;
}
viewable_types::Type Response::getViewableTypeId() const
{
    return viewable_types::RESPONSE;
}

Response::~Response()
{
}

