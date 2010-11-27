#ifndef __LUA_HPP__
#define __LUA_HPP__

extern "C"
{
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}
#include <luabind/class.hpp>

#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModuleManager.hpp"
#include "api/IModule.hpp"
#include "LuaApi.hpp"

class Lua : public zhttpd::api::IModule
{
    private:
        bool _executeLua(std::string const& file);
        void _luaError(zhttpd::api::IRequest* request, std::string const& file);
        void _loadApi(zhttpd::api::IRequest* request);
        void _parseQuery(zhttpd::api::IRequest* request);
        void _parsePost(zhttpd::api::IRequest* request);
        char _urlDecode(std::string::const_iterator& it, std::string::const_iterator& itEnd);
        lua_State* _state;
        bool _loaded;
        std::string _post;
    public:
        Lua(zhttpd::api::IModuleManager* manager);
        ~Lua();
        bool processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer);
};

#endif

