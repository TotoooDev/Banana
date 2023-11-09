#pragma once
#include <Scene/Components.h>
#include <Classes/LuaTransform.h>
#include <LuaClass.h>

namespace Banana
{
	class LuaDirectionnalLight
	{
	public:
		static void Bind(lua_State* L)
		{
			LuaClass<LuaDirectionnalLight> luaClass(L, "DirectionnalLight");

		}

	public:


	private:
		DirectionalLight m_Light;
	};
}