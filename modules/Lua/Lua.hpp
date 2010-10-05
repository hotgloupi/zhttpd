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

class Lua : public ZHTTPD::API::IModule
{
    private:
        bool _executeLua(std::string const& file);
        void _luaError(ZHTTPD::API::IRequest* request, std::string const& file);
        void _loadApi(ZHTTPD::API::IRequest* request);
        void _parseQuery(ZHTTPD::API::IRequest* request);
        void _parsePost(ZHTTPD::API::IRequest* request);
        char _urlDecode(std::string::const_iterator& it, std::string::const_iterator& itEnd);
        lua_State* _state;
        bool _loaded;
        std::string _post;
    public:
        Lua(ZHTTPD::API::IModuleManager* manager);
        ~Lua();
        bool processRequest(ZHTTPD::API::EVENT::Type event, ZHTTPD::API::IRequest* request, ZHTTPD::API::IBuffer* buffer);
};

#endif

