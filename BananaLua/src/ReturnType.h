#pragma once
#include <Concepts.h>

namespace Banana
{
	// Base SignatureReturnType class
	template <typename Sig>
	struct SignatureReturnType;

	// Specialization for functions
	template <typename ReturnType, typename... Args>
	struct SignatureReturnType<ReturnType(Args...)>
	{
		using Type = ReturnType;
		static ReturnType GetType() { return Type(); }
	};

	// Specialization for member functions
	template <typename ReturnType, typename Object, typename... Args>
	struct SignatureReturnType<ReturnType(Object::*)(Args...)>
	{
		using Type = ReturnType;
		static Type GetType() { return Type(); }
	};

	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A variable of the return type of the function passed in template parameter.
	 * @param Func - The function to get the types from.
	 */
	template <IsFunction Func>
	auto GetReturnType(const Func& t) -> SignatureReturnType<Func>::Type
	{
		return SignatureReturnType<Func>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A variable of the return type of the functor passed in template parameter.
	 * @param Func - The functor to get the types from.
	 */
	template <IsFunctor Functor>
	auto GetReturnType(Functor&& t) -> SignatureReturnType<decltype(&std::decay_t<Functor>::operator())>::Type
	{
		return SignatureReturnType<decltype(&std::decay_t<Functor>::operator())>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A variable of the return type of the functor passed in template parameter.
	 * @param Func - The functor to get the types from.
	 */
	template <IsFunctor Functor>
	auto GetReturnType(const Functor& t) -> SignatureReturnType<decltype(&std::decay_t<Functor>::operator())>::Type
	{
		return SignatureReturnType<decltype(&std::decay_t<Functor>::operator())>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A variable of the return type of the member function passed in template parameter.
	 * @param Func - The member function to get the types from.
	 */
	template <IsMemberFunction MemberFunc>
	auto GetReturnType(MemberFunc&& t) -> SignatureReturnType<std::decay_t<MemberFunc>>::Type
	{
		return SignatureReturnType<std::decay_t<MemberFunc>>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A variable of the return type of the member function passed in template parameter.
	 * @param Func - The member function to get the types from.
	 */
	template <IsMemberFunction MemberFunc>
	auto GetReturnType(const MemberFunc& t) -> SignatureReturnType<std::decay_t<MemberFunc>>::Type
	{
		return SignatureReturnType<std::decay_t<MemberFunc>>::GetType();
	}
}