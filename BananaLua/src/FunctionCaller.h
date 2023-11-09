#pragma once

// Base struct
template <typename Sig>
struct FunctionCaller;

// Function overload
template <typename ReturnType, typename... Args>
struct FunctionCaller<ReturnType(Args...)>
{
	FunctionCaller(ReturnType func(Args...))
	{
		Function = func;
	}

	ReturnType(*Function)(Args...);
};

// Member function overload
template <typename ReturnType, typename Object, typename... Args>
struct FunctionCaller<ReturnType(Object::*)(Args...)>
{
	FunctionCaller(ReturnType(Object::*func)(Args...))
	{
		Function = func;
	}

	ReturnType(Object::*Function)(Args...);
};