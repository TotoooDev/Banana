#pragma once
#include <Lua/Log.h>
#include <lua/lua.hpp>

/**
 * @brief A helper to bind a C++ class to Lua. In the docs I'll refer to the binding as the table-class (since in Lua it's just a table).
 */
template <typename T>
class LuaClass
{
public:
	/**
	 * @brief Allocates memory for a table-class in Lua that you can use to perform cool operations. The table will be globally accessible to
	 *        create new instances of your table-class using `foo = Foo.new()` in Lua.
	 *        A default constructor will be created but note that it does not call C++ constructor! It just allocates memory.
	 *        You can add your own constructors that will call the C++ constructor.
	 * @param state - The Lua state where you want your table-class to be allocated.
	 * @param name - The name of your global table-class.
	 */
	LuaClass(lua_State* state, const std::string& name)
		: m_State(state), m_Name(name)
	{
		// Create the global table
		lua_newtable(m_State);
		
		// Set the default constructor that does nothing but create the userdatum
		lua_pushcfunction(m_State, [](lua_State* L) -> int 
			{
				T* luaClass = (T*)lua_newuserdata(L, sizeof(T));
				return 1; 
			});
		lua_setfield(m_State, -2, "new");

		lua_setglobal(m_State, m_Name.c_str());
	}

	/**
	 * Adds a constructor for the class. For now only 3 types are handled: float, bool and std::string.
	 * @param Args... - The types of the parameters of the constructor. They are passed in the template arguments. You can safely pass no types.
	 * @param constructorName - The name of your constructor. It defaults to "new".
	 */
	template <typename... Args>
	void SetConstructor(const std::string& constructorName = "new")
	{
		lua_getglobal(m_State, m_Name.c_str());
		
		// Set the `new` function for the table to the type processing method
		lua_pushcfunction(m_State, LuaConstructor<Args...>);
		lua_setfield(m_State, -2, constructorName.c_str());

		lua_setglobal(m_State, m_Name.c_str());
	}


// Type processing for constructor
private:
	template <typename... Args>
	static int LuaConstructor(lua_State* L)
	{
		// This is magical to me, here we go to a specialization of the method that takes an unsigned int and the argumets in the template.
		// The unsigned int is the index of the type we want to process, so here it is 0 for the first type in the list.
		// I rarely use templates, how can you tell?
		T* luaClass = (T*)lua_newuserdata(L, sizeof(T));
		ProcessConstructorTypes<0, Args...>(L);

		// Call the constructor because Lua just allocated the memory
		new(luaClass) T(); // Set the constructor parameters here but I don't know how :(

		return 1;
	}

	template<unsigned int CurrentIndex, typename... Args>
	static void ProcessConstructorTypes(lua_State* L)
	{
		// If we have processed all the types, we do nothing
		if constexpr(CurrentIndex < sizeof...(Args))
		{
			// Get the type at the current index and process it
			using CurrentType = std::tuple_element_t<CurrentIndex, std::tuple<Args...>>;
			ProcessConstructorType<CurrentType>(L, CurrentIndex + 1);

			// And BAM, recursive magic!
			// We increment the type index so we know where we are in the type list
			ProcessConstructorTypes<CurrentIndex + 1, Args...>(L);
		}
	}

	template <typename T>
	static void ProcessConstructorType(lua_State* L, unsigned int index)
	{
		BANANA_LUA_WARN("Unhandled type {} in constructor!", typeid(T).name());
	}

	template <>
	static void ProcessConstructorType<float>(lua_State* L, unsigned int index)
	{
		double n = luaL_checknumber(L, index);
		BANANA_LUA_INFO("Processing number: {}", n);
	}

	template <>
	static void ProcessConstructorType<bool>(lua_State* L, unsigned int index)
	{
		// There is no luaL_checkboolean in the Lua library :(
		luaL_argcheck(L, lua_isboolean(L, index), 1, "boolean expected");
		bool b = lua_toboolean(L, index);
		BANANA_LUA_INFO("Processing boolean: {}", b);
	}

	template <>
	static void ProcessConstructorType<std::string>(lua_State* L, unsigned int index)
	{
		const char* str = luaL_checkstring(L, index);
		BANANA_LUA_INFO("Processing string: {}", str);
	}
	
private:
	lua_State* m_State;
	std::string m_Name;
};