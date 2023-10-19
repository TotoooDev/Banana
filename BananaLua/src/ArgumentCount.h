#pragma once
#include <Concepts.h>

namespace Banana
{
	// Base SignatureArgCount class
	template <typename Sig>
	struct SignatureArgCount;

	// Specialization for functions
	template <typename ReturnType, typename... Args>
	struct SignatureArgCount<ReturnType(Args...)>
	{
		static unsigned int GetArgCount() { return sizeof...(Args); }
	};

	// Specialization for member functions
	template <typename ReturnType, typename Object, typename... Args>
	struct SignatureArgCount<ReturnType(Object::*)(Args...)>
	{
		static unsigned int GetArgCount() { return sizeof...(Args); }
	};

	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns The number of arguments of the function.
	 * @param Func - The function to get argument count from.
	 */
	template <IsFunction Func>
	unsigned int GetArgCount(const Func& t)
	{
		return SignatureArgCount<Func>::GetArgCount();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns The number of arguments of the functor.
	 * @param Func - The functor to get argument count from.
	 */
	template <IsFunctor Functor>
	unsigned int GetArgCount(Functor&& t)
	{
		return SignatureArgCount<decltype(&std::decay_t<Functor>::operator())>::GetArgCount();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns The number of arguments of the functor.
	 * @param Func - The functor to get argument count from.
	 */
	template <IsFunctor Functor>
	unsigned int GetArgCount(const Functor& t)
	{
		return SignatureArgCount<decltype(&std::decay_t<Functor>::operator())>::GetArgCount();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns The number of arguments of the member function.
	 * @param Func - The member function to get argument count from.
	 */
	template <IsMemberFunction MemberFunc>
	unsigned int GetArgCount(MemberFunc&& t)
	{
		return SignatureArgCount<std::decay_t<MemberFunc>>::GetArgCount();
	}
	/**
	 * From https://stackoverflow.com/a/70954691
	 * @returns The number of arguments of the member function.
	 * @param Func - The member function to get argument count from.
	 */
	template <IsMemberFunction MemberFunc>
	unsigned int GetArgCount(const MemberFunc& t)
	{
		return SignatureArgCount<std::decay_t<MemberFunc>>::GetArgCount();
	}
}