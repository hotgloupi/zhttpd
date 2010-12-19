#ifndef __LUAMANAGER_HPP__
#define __LUAMANAGER_HPP__

#include <string>

#include "server/modules/common/AbstractManager.hpp"

#include "Lua.hpp"

class LuaManager : public zhttpd::mod::StatefullManager<Lua>
{
    private:
        std::string _name;
    public:
        LuaManager() :
            zhttpd::mod::StatefullManager<Lua>("mod_lua", zhttpd::api::category::PROCESSING)
        {
        }

        bool isRequired(zhttpd::api::IRequest const& request) const
        {
            char const* f = request.getFilePath().c_str();
            zhttpd::api::size_t s = request.getFilePath().size();

            if (s < 4 || f[s - 1] != 'a' || f[s - 2] != 'u' || f[s -3] != 'l' || f[s - 4] != '.')
                return false;
            return true;
        }
};

#endif

