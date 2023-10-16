#include <Core/Config.h>
#include <Log.h>
#include <LuaClass.h>
#include <lua/lua.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef BANANA_WINDOWS
	#define BANANA_EXPORT __declspec(dllexport)
#else
	#define BANANA_EXPORT
#endif

extern "C"
{
	static const luaL_Reg BananaLib[] =
	{
		// Logging
		{ "LogInfo", lua_LogInfo },
		{ "LogWarn", lua_LogWarn },
		{ "LogError", lua_LogError },

		{ nullptr, nullptr }
	};

	struct Foo
	{
		Foo(float bar, bool what, std::string how) : Bar(bar), What(what), How(how) { BANANA_LUA_INFO("Other constructor is called!"); }
		int CoolMethod() { BANANA_LUA_INFO("Cool method!"); return 0; }
		float Bar; bool What; std::string How;
	};

	BANANA_EXPORT int luaopen_Banana(lua_State* L)
	{
		// Set the logger
		auto log = spdlog::stdout_color_mt("lua");
		log->set_pattern("[%D %T] %^[%l] [LUA]%$ %v");

		LuaClass<Foo> yipee(L, "Foo");
		yipee.SetConstructor<float, bool, std::string>();
		yipee.SetFunction("Bar", &Foo::CoolMethod);
		
		// Load the library into Lua
		lua_newtable(L);
		luaL_setfuncs(L, BananaLib, 0);

		return 1;
	}
}