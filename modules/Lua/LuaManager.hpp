#ifndef __LUAMANAGER_HPP__
#define __LUAMANAGER_HPP__

#include <string>

#include "server/modules/common/AbstractManager.hpp"

#include "Lua.hpp"

class LuaManager : public ZHTTPD::MOD::StatefullManager<Lua>
{
    private:
        std::string _name;
    public:
        LuaManager() :
            ZHTTPD::MOD::StatefullManager<Lua>("mod_lua", ZHTTPD::API::CATEGORY::PROCESSING)
        {
        }

        bool isRequired(ZHTTPD::API::IRequest const& request) const
        {
            std::string ext;
            std::string::const_reverse_iterator it = request.getFilePath().rbegin();
            std::string::const_reverse_iterator itEnd = request.getFilePath().rend();
            for (; it != itEnd; ++it)
                if (*it == '.')
                {
                    if (ext == "lua")
                        return true;
                    return false;
                }
                else
                    ext.insert(ext.begin(), *it);
            return false;
        }
};

#endif

