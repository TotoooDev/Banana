#pragma once
#include <type_traits>

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
