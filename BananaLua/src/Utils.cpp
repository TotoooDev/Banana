#include <Utils.h>
#include <Lua/Log.h>
#include <lua/lua.hpp>

namespace Banana
{
    void DumpStack(lua_State* L)
    {
        BANANA_LUA_INFO("Beginning stack dump!");

        int top = lua_gettop(L);
        for (int i = 1; i <= top; i++)
        {
            std::string str = std::to_string(i) + ": " + std::string(luaL_typename(L, i)) + " = ";
            switch (lua_type(L, i))
            {
            case LUA_TNUMBER:
                str += std::to_string(lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                str += lua_tostring(L, i);
                break;
            case LUA_TBOOLEAN:
                str += lua_toboolean(L, i) ? "true" : "false";
                break;
            case LUA_TNIL:
                str += "nil";
                break;
            default:
                str += std::to_string((unsigned int)lua_topointer(L, i));
                break;
            }

            BANANA_LUA_INFO(str);
        }

        BANANA_LUA_INFO("Stack dump over.");
    }
}