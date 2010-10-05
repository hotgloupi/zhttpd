
#include <iostream>

#include "LuaApi.hpp"

LuaApi::LuaApi(ZHTTPD::API::IRequest* request) :
    _request(request)
{
}

LuaApi::LuaApi(LuaApi const& api) :
    _request(api._request), _buffer(api._buffer)
{
}

LuaApi::~LuaApi()
{
    if (this->_buffer.size() > 0)
        this->_request->giveData(this->_request->getBufferManager().allocate(this->_buffer.data(), this->_buffer.size()));
}

LuaApi& LuaApi::operator =(LuaApi const& api)
{
    this->_request = api._request;
    this->_buffer = api._buffer;
    return *this;
}

void LuaApi::sleep(int sec) const
{
#ifdef WIN32
    ::Sleep(sec * 1000);
#else // WIN32
	::sleep(sec);
#endif // !WIN32
}

void LuaApi::set_header(std::string const& field, std::string const& value)
{
    this->_request->setResponseHeader(field, value);
}

std::string const& LuaApi::get_header(std::string const& field) const
{
    return this->_request->getRequestHeader(field);
}

void LuaApi::flush()
{
    if (this->_buffer.size() > 0)
        this->_request->giveData(this->_request->getBufferManager().allocate(this->_buffer.data(), this->_buffer.size()));
	this->_buffer.clear();
}

void LuaApi::echo(std::string const& data)
{
    this->_buffer += data;
    if (this->_buffer.size() >= 4096)
    {
        this->_request->giveData(this->_request->getBufferManager().allocate(this->_buffer.data(), this->_buffer.size()));
        this->_buffer.clear();
    }
}

