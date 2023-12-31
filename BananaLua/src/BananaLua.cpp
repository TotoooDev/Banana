#include <Core/Config.h>
#include <Log.h>
#include <LuaClass.h>
#include <Classes/LuaTransform.h>
#include <lua/lua.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef BANANA_WINDOWS
	// microsoft the type of guy to specify stupid keywords
	#define BANANA_EXPORT __declspec(dllexport)
#else
	#define BANANA_EXPORT
#endif

extern "C"
{
	namespace Banana
	{
		static const luaL_Reg BananaLib[] =
		{
			{ "GetTransform", LuaTransform::GetComponent },

			// Logging
			{ "LogInfo", lua_LogInfo },
			{ "LogWarn", lua_LogWarn },
			{ "LogError", lua_LogError },

			{ nullptr, nullptr }
		};

		BANANA_EXPORT int luaopen_Banana(lua_State* L)
		{
			// Set the logger
			auto log = spdlog::stdout_color_mt("lua");
			log->set_pattern("[%D %T] %^[%l] [LUA]%$ %v");

			Vec3::Bind(L);
			LuaTransform::Bind(L);

			// Load the library into Lua
			lua_newtable(L);
			luaL_setfuncs(L, BananaLib, 0);

			return 1;
		}
	}
}