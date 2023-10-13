#pragma once
#include <memory>

// Once again shamelessly borrowed from Cherno
namespace Banana
{
	/**
	 * @brief A smart pointer type that only exists within a scope.
	 */
	template <typename T>
	using Scope = std::unique_ptr<T>;

	/**
	 * @brief Creates a Scope smart pointer.
	 * @returns The Scope smart pointer.
	 */
	template <typename T, typename... Args>
	Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	/**
	 * @brief A smart pointer type that has a different name.
	 */
	template <typename T>
	using Ref = std::shared_ptr<T>;

	/**
	 * @brief Creates a smart pointer.
	 * @returns The smart pointer.
	 */
	template <typename T, typename... Args>
	Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}