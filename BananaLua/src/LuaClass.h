#pragma once
#include <Lua/Log.h>
#include <lua/lua.hpp>
#include <ArgumentTypes.h>

// No, working with templates and meta programming is not fun.
template <typename F>
concept IsFun = std::is_function_v<F>;

void foo(const std::string&, int, double) {}

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
	 * Adds a constructor for the table-class. For now only 3 types are handled: float, bool and std::string.
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

	/**
	 * @brief Adds a function (method?) for th table-class.
	 * @param name - The name of the function.
	 * @param func - The function to call.
	 */
	template <typename Func>
	void SetFunction(const std::string& name, Func func)
	{
		BANANA_LUA_INFO(typeid(foo).name());
		BANANA_LUA_INFO(typeid(func).name());

		auto argumentTypes = GetArgumentTypes(foo);

		// using ReturnType = decltype(std::function{ func })::result_type;
		// BANANA_LUA_INFO(typeid(Func).name());

		lua_getglobal(m_State, m_Name.c_str());

		lua_pushcfunction(m_State, [](lua_State* L) -> int
			{
				return 0;
			});
		lua_setfield(m_State, -2, name.c_str());

		lua_setglobal(m_State, m_Name.c_str());
	}


// Type processing for constructor
private:
	template <typename... Args>
	static int LuaConstructor(lua_State* L)
	{
		T* luaClass = (T*)lua_newuserdata(L, sizeof(T));
		// This is magical to me, here we go to a specialization of the method that takes an unsigned int and the argumets in the template.
		// The unsigned int is the index of the type we want to process, so here it is 0 for the first type in the list.
		// I rarely use templates, how can you tell?
		std::tuple<Args...> args;
		ProcessConstructorTypes<0, Args...>(L, args);

		// Call the constructor because Lua just allocated the memory
		// std::make_from_tuple creates a class of type T with arguments as tuples
		*luaClass = std::make_from_tuple<T>(args);

		return 1;
	}

	template<unsigned int CurrentIndex, typename... Args>
	static void ProcessConstructorTypes(lua_State* L, std::tuple<Args...>& argumentsTuple)
	{
		// If we have processed all the types, we do nothing
		if constexpr(CurrentIndex < sizeof...(Args))
		{
			// Get the type at the current index and process it
			using CurrentType = std::tuple_element_t<CurrentIndex, std::tuple<Args...>>;
			CurrentType arg;
			ProcessConstructorType<CurrentType>(L, &arg, CurrentIndex + 1);
			std::get<CurrentIndex>(argumentsTuple) = arg;

			// And BAM, recursive magic!
			// We increment the type index so we know where we are in the type list
			ProcessConstructorTypes<CurrentIndex + 1, Args...>(L, argumentsTuple);
		}
	}

	template <typename T>
	static void ProcessConstructorType(lua_State* L, T* var, unsigned int index)
	{
		BANANA_LUA_WARN("Unhandled type {} in constructor!", typeid(T).name());
	}

	template <>
	static void ProcessConstructorType<float>(lua_State* L, float* var, unsigned int index)
	{
		double n = luaL_checknumber(L, index);
		*var = (float)n;
	}

	template <>
	static void ProcessConstructorType<bool>(lua_State* L, bool* var, unsigned int index)
	{
		// There is no luaL_checkboolean in the Lua library :(
		luaL_argcheck(L, lua_isboolean(L, index), 1, "boolean expected");
		bool b = lua_toboolean(L, index);
		*var = b;
	}

	template <>
	static void ProcessConstructorType<std::string>(lua_State* L, std::string* var, unsigned int index)
	{
		const char* str = luaL_checkstring(L, index);
		*var = str;
	}

// Functions stuff
private:
	template <typename R, typename... Types>
	constexpr unsigned int GetArgumentCount(R(*f)(Types...))
	{
		return sizeof...(Types);
	}
	
private:
	lua_State* m_State;
	std::string m_Name;
};