#pragma once
#include <tuple>

// From https://stackoverflow.com/a/70954691

// Base Signature structure
template <typename Sig>
struct Signature;

// Specialization for functions
template <typename ReturnType, typename... Args>
struct Signature<ReturnType(Args...)>
{
	using Type = std::tuple<Args...>;
};

// Specialization for member functions
template <typename ReturnType, typename Object, typename... Args>
struct Signature<ReturnType(Object::*)(Args...) const>
{
	using Type = std::tuple<Args...>;
};

// I don't understand what the fuck is going on here
template <typename Function>
concept IsFunction = std::is_function_v<Function>;

template <typename Function>
concept IsMemberFunction = std::is_member_function_pointer_v<std::decay_t<Function>>;

template <typename Function>
concept IsFunctor = std::is_class_v<std::decay_t<Function>> && requires(Function && t)
{
	&std::decay_t<Function>::operator();
};

template <IsFunction T>
auto GetArgumentTypes(const T& t) -> Signature<T>::Type;

template <IsFunctor T>
auto GetArgumentTypes(T&& t) -> Signature<decltype(&std::decay_t<T>::operator())>::Type;

template <IsFunctor T>
auto GetArgumentTypes(const T& t) -> Signature<decltype(&std::decay_t<T>::operator())>::Type;

template <IsMemberFunction T>
auto GetArgumentTypes(T&& t) -> Signature<std::decay_t<T>>::Type;

template <IsMemberFunction T>
auto GetArgumentTypes(const T& t) -> Signature<std::decay_t<T>>::Type;