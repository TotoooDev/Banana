#pragma once
#include <Concepts.h>
#include <tuple>

namespace Banana
{
	// Base SignatureArgTypes structure
	template <typename Sig>
	struct SignatureArgTypes;

	// Specialization for functions
	template <typename ReturnType, typename... Args>
	struct SignatureArgTypes<ReturnType(Args...)>
	{
		using Type = std::tuple<Args...>;
		static Type GetType() { return Type(); }
	};

	// Specialization for member functions
	template <typename ReturnType, typename Object, typename... Args>
	struct SignatureArgTypes<ReturnType(Object::*)(Args...)>
	{
		using Type = std::tuple<Args...>;
		static Type GetType() { return Type(); }
	};

	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A tuple of the types of the function passed in template parameter.
	 * @param Func - The function to get the types from.
	 */
	template <IsFunction Func>
	auto GetArgumentTypes(const Func& t) -> SignatureArgTypes<Func>::Type
	{
		return SignatureArgTypes<Func>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A tuple of the types of the functor passed in template parameter.
	 * @param Func - The functor to get the types from.
	 */
	template <IsFunctor Functor>
	auto GetArgumentTypes(Functor&& t) -> SignatureArgTypes<decltype(&std::decay_t<Functor>::operator())>::Type
	{
		return SignatureArgTypes<decltype(&std::decay_t<Functor>::operator())>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A tuple of the types of the functor passed in template parameter.
	 * @param Func - The functor to get the types from.
	 */
	template <IsFunctor Functor>
	auto GetArgumentTypes(const Functor& t) -> SignatureArgTypes<decltype(&std::decay_t<Functor>::operator())>::Type
	{
		return SignatureArgTypes<decltype(&std::decay_t<Functor>::operator())>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A tuple of the types of the member function passed in template parameter.
	 * @param Func - The member function to get the types from.
	 */
	template <IsMemberFunction MemberFunc>
	auto GetArgumentTypes(MemberFunc&& t) -> SignatureArgTypes<std::decay_t<MemberFunc>>::Type
	{
		return SignatureArgTypes<std::decay_t<MemberFunc>>::GetType();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns A tuple of the types of the member function passed in template parameter.
	 * @param Func - The member function to get the types from.
	 */
	template <IsMemberFunction MemberFunc>
	auto GetArgumentTypes(const MemberFunc& t) -> SignatureArgTypes<std::decay_t<MemberFunc>>::Type
	{
		return SignatureArgTypes<std::decay_t<MemberFunc>>::GetType();
	}
}