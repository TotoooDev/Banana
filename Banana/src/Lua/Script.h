#pragma once
#include <Scene/Entity.h>
#include <string>
#include <lua/lua.hpp>

namespace Banana
{
	/**
	 * STILL IN DEVELOPMENT, THIS IS UNSTABLE AND SUBJECT TO CHANGE
	 * A wrapper around a Lua script.
	 * You need to define two functions for the script to run properly:
	 * - `OnStart` gets called once when the script is created.
	 * - `OnUpdate` gets called every loop.
	 * See the BananaLua documentation dor more details.
	 */
	class Script
	{
	public:
		static void Init();
	
	public:
		Script(const std::string& path, Entity ent);
		~Script();

		void Run();
		void OnStart();
		void OnUpdate(double timestep);

		bool HasStarted() { return m_HasStarted; }
		bool HasError() { return m_HasError; }

		lua_State* GetState() { return m_State; }

	private:
		lua_State* m_State;
		bool m_HasStarted = false;
		bool m_HasError = false;
	};
}