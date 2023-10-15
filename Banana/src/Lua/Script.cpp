#include <Lua/Log.h>
#include <Lua/Script.h>
#include <lua/lua.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Banana
{
	void Script::Init()
	{
		auto log = spdlog::stdout_color_mt("lua");
		log->set_pattern("[%D %T] %^[%l] [LUA]%$ %v");
	}

	Script::Script(const std::string& path)
	{
		// Create the Lua state and open the standard libraries
		m_State = luaL_newstate();
		if (!m_State)
			BANANA_LUA_ERROR("Failed to create lua state!");

		luaL_openlibs(m_State);

		// Load the script file
		BANANA_LUA_INFO("Loading script {}...", path);
		int ret = luaL_loadfile(m_State, path.c_str());
		if (ret != LUA_OK)
			BANANA_LUA_ERROR("Failed to load {}!", path);
	}
	
	Script::~Script()
	{
		lua_close(m_State);
	}

	void Script::Run()
	{
		int ret = lua_pcall(m_State, 0, 0, 0);
		if (ret != LUA_OK)
			BANANA_LUA_ERROR("Failed to run the lua script!");
	}

	double Script::GetNumber(const std::string& name)
	{
		lua_getglobal(m_State, name.c_str());
		if (!lua_isnumber(m_State, -1))
			BANANA_LUA_ERROR("{} should be a number!", name);
		return lua_tonumber(m_State, -1);
	}
}