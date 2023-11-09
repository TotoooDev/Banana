#pragma once
#include <Lua/Log.h>
#include <lua/lua.hpp>
#include <spdlog/spdlog.h>

extern "C"
{
	static int lua_LogInfo(lua_State* L)
	{
		const char* str = luaL_tolstring(L, 1, nullptr);
		BANANA_LUA_INFO(str);
		return 0;
	}

	static int lua_LogWarn(lua_State* L)
	{
		const char* str = luaL_tolstring(L, 1, nullptr);
		BANANA_LUA_WARN(str);
		return 0;
	}

	static int lua_LogError(lua_State* L)
	{
		const char* str = luaL_tolstring(L, 1, nullptr);
		BANANA_LUA_ERROR(str);
		return 0;
	}
}