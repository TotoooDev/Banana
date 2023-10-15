#pragma once
#include <string>
#include <lua/lua.hpp>

namespace Banana
{
	// For testing purposes only
	class Script
	{
	public:
		Script(const std::string& path);
		~Script();

		static void Init();

		void Run();

		double GetNumber(const std::string& name);

	private:
		lua_State* m_State;
	};
}