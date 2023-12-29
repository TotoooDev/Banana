#include <Lua/Log.h>
#include <Lua/Script.h>
#include <lua/lua.hpp>
#include <Classes/LuaTransform.h>
#include <Utils.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Banana
{
	void Script::Init()
	{
		auto log = spdlog::stdout_color_mt("lua");
		log->set_pattern("[%D %T] %^[%l] [LUA]%$ %v");
	}

	Script::Script(const std::string& path, Entity ent)
	{
		// Create the Lua state and open the standard libraries
		m_State = luaL_newstate();
		if (!m_State)
			BANANA_LUA_ERROR("Failed to create lua state!");

		luaL_openlibs(m_State);

		// Set the entity userdatum
		Entity* entUserdata = (Entity*)lua_newuserdata(m_State, sizeof(Entity));
		*entUserdata = ent;
		luaL_newmetatable(m_State, typeid(Entity).raw_name());
		lua_setmetatable(m_State, -2);
		lua_setglobal(m_State, "Entity");

		// Load the script file
		BANANA_LUA_INFO("Loading script {}...", path);
		int ret = luaL_dofile(m_State, path.c_str());
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
		{
			BANANA_LUA_ERROR("Failed to run the lua script!");
			BANANA_LUA_ERROR(lua_tostring(m_State, -1));
			m_HasError = true;
		}
	}

	void Script::OnStart()
	{
		lua_getglobal(m_State, "OnStart");
		int ret = lua_pcall(m_State, 0, 0, 0);
		if (ret != LUA_OK)
		{
			BANANA_LUA_ERROR("Failed to run the lua script!");
			BANANA_LUA_ERROR(lua_tostring(m_State, -1));
			m_HasError = true;
		}
		m_HasStarted = true;
	}

	void Script::OnUpdate(double timestep)
	{
		lua_getglobal(m_State, "OnUpdate");
		lua_pushnumber(m_State, timestep);
		int ret = lua_pcall(m_State, 1, 0, 0);
		if (ret != LUA_OK)
		{
			BANANA_LUA_ERROR("Failed to run the lua script!");
			BANANA_LUA_ERROR(lua_tostring(m_State, -1));
			m_HasError = true;
		}
	}

	void Script::DumpStack()
	{
		BANANA_LUA_INFO("Beginning stack dump!");

		int top = lua_gettop(m_State);
		for (int i = 1; i <= top; i++)
		{
			std::string str = std::to_string(i) + ": " + std::string(luaL_typename(m_State, i)) + " = ";
			switch (lua_type(m_State, i))
			{
			case LUA_TNUMBER:
				str += std::to_string(lua_tonumber(m_State, i));
				break;
			case LUA_TSTRING:
				str += lua_tostring(m_State, i);
				break;
			case LUA_TBOOLEAN:
				str += lua_toboolean(m_State, i) ? "true" : "false";
				break;
			case LUA_TNIL:
				str += "nil";
				break;
			default:
				str += std::to_string((unsigned int)lua_topointer(m_State, i));
				break;
			}

			BANANA_LUA_INFO(str);
		}

		BANANA_LUA_INFO("Stack dump over.");
	}
}