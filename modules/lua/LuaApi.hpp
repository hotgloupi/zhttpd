#ifndef __LUAAPI_HPP__
#define __LUAAPI_HPP__

#include <string>

#include "api/IRequest.hpp"

class LuaApi
{
    private:
        zhttpd::api::IRequest* _request;
        std::string _buffer;
    public:
        LuaApi(zhttpd::api::IRequest* _request);
        LuaApi(LuaApi const& api);
        ~LuaApi();
        LuaApi& operator =(LuaApi const& api);
        void set_header(std::string const& field, std::string const& value);
        std::string const& get_header(std::string const& field) const;
        void echo(std::string const& data);
		void sleep(int sec) const;
		void flush();
};

#endif

