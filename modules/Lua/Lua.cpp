#include "api/constants.hpp"
#include "api/IBuffer.hpp"
#include "api/IRequest.hpp"
#include "api/IModule.hpp"
#include "utils/Logger.hpp"

#include "Lua.hpp"

Lua::Lua(zhttpd::api::IModuleManager*)
{
    this->_state = lua_open();
    lua_pushcfunction(this->_state, luaopen_base);
    lua_pushstring(this->_state, "");
    lua_call(this->_state, 1, 0);
    lua_pushcfunction(this->_state, luaopen_string);
    lua_pushstring(this->_state, LUA_STRLIBNAME);
    lua_call(this->_state, 1, 0);
    lua_pushcfunction(this->_state, luaopen_table);
    lua_pushstring(this->_state, LUA_TABLIBNAME);
    lua_call(this->_state, 1, 0);
    lua_pushcfunction(this->_state, luaopen_io);
    lua_pushstring(this->_state, LUA_IOLIBNAME);
    lua_call(this->_state, 1, 0);
    lua_pushcfunction(this->_state, luaopen_math);
    lua_pushstring(this->_state, LUA_MATHLIBNAME);
    lua_call(this->_state, 1, 0);
    luabind::open(this->_state);
}

Lua::~Lua()
{
    if (this->_state != 0)
        lua_close(this->_state);
}

bool Lua::processRequest(zhttpd::api::event::Type event, zhttpd::api::IRequest* request, zhttpd::api::IBuffer* buffer)
{
    if (event == zhttpd::api::event::ON_END)
    {
        request->setResponseHeader("Content-Type", "text/html");
        request->setResponseCode(zhttpd::api::http_code::OK);
        this->_loadApi(request);
        std::string file = request->getFilePath();
        if (this->_executeLua(file))
        {
            lua_close(this->_state);
            this->_state = 0;
            request->raiseEnd();
            return true;
        }
        this->_luaError(request, file);
        return true;
    }
    else if (event == zhttpd::api::event::ON_REQUEST_DATA)
    {
        this->_post.append(buffer->getRawData(), buffer->getSize());
        request->getBufferManager().release(buffer);
        return true;
    }
    return false;
}

char Lua::_urlDecode(std::string::const_iterator& it, std::string::const_iterator& itEnd)
{
    if (*it != '%')
        return *it;
    if (++it != itEnd)
    {
#define TO_HEX(x) x >= '0' && x <= '9' ? x - '0' : x >= 'A' && x <= 'F' ? x - 'A' + 10 : x >= 'a' && x <= 'f' ? x - 'a' + 10 : x
        char digit = (TO_HEX(*it)) * 16;
        if (++it != itEnd)
        {
            digit += TO_HEX(*it);
            return digit;
        }
#undef TO_HEX
    }
    --it;
    return '%';
}

void Lua::_parsePost(zhttpd::api::IRequest* request)
{
}

void Lua::_parseQuery(zhttpd::api::IRequest* request)
{
    bool got_question = false;
    bool got_equal = false;
    std::string field;
    std::string value;
    std::string::const_iterator it = request->getRequestQuery().begin();
    std::string::const_iterator itEnd = request->getRequestQuery().end();
    for (; it != itEnd; ++it)
        if (*it == '?' && !got_question)
            got_question = true;
        else if (got_question)
        {
            if (*it == '&')
            {
                if (field.size() > 0)
                    luabind::globals(this->_state)["_GET"][field] = value;
                field.clear();
                value.clear();
                got_equal = false;
            }
            else if (*it == '=')
                got_equal = true;
            else if (got_equal)
                value += this->_urlDecode(it, itEnd);
            else
                field += this->_urlDecode(it, itEnd);
        }
    if (field.size() > 0)
        luabind::globals(this->_state)["_GET"][field] = value;
}

void Lua::_loadApi(zhttpd::api::IRequest* request)
{
    luabind::module(this->_state)
        [
        luabind::class_<LuaApi>("__api__")
        .def(luabind::constructor<zhttpd::api::IRequest*>())
        .def("set_header", &LuaApi::set_header)
        .def("get_header", &LuaApi::get_header)
        .def("echo", &LuaApi::echo)
        .def("sleep", &LuaApi::sleep)
        .def("flush", &LuaApi::flush)
        ];
    luabind::globals(this->_state)["_"] = LuaApi(request);
    luabind::globals(this->_state)["_GET"] = luabind::newtable(this->_state);
    this->_parseQuery(request);
    luabind::globals(this->_state)["_POST"] = luabind::newtable(this->_state);
    this->_parsePost(request);
}

void Lua::_luaError(zhttpd::api::IRequest* request, std::string const& file)
{
    request->setResponseHeader("Content-Type", "text/html");
    std::string error;
    error += "<p><strong>Lua error while processing file \"" + file + "\":</strong> ";
    error += lua_tostring(this->_state, -1);
    error += "</p>";
    lua_close(this->_state);
    this->_state = 0;
    zhttpd::api::IBuffer* str = request->getBufferManager().allocate(error.c_str());
    request->giveData(str);
    request->raiseEnd();
}

bool Lua::_executeLua(std::string const& file)
{
    return (luaL_loadfile(this->_state, file.c_str()) || lua_pcall(this->_state, 0, LUA_MULTRET, 0)) == 0;
}

