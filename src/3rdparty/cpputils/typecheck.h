#pragma once
#include <type_traits>
#include <variant>

namespace cpputils
{
	template<typename type, typename object>
	inline bool isinstance(const object& obj)
	{
		return typeid(obj) == typeid(type);
	}

	template<typename T, typename = void>
	struct is_std_container : std::false_type {};

	template<typename T>
	struct is_std_container<T,
		std::void_t<decltype(std::declval<T>().begin()),
		decltype(std::declval<T>().end()),
		typename T::value_type >> : std::true_type {};// will  be enabled for iterable objects

	template<typename T> struct is_variant : std::false_type {};

	template<typename ...Args>
	struct is_variant<std::variant<Args...>> : std::true_type {};

	template<typename T>
	inline constexpr bool is_variant_v = is_variant<T>::value;

	template< typename C, typename = void >
	struct has_reserve : std::false_type {};

	template< typename C >
	struct has_reserve< C, typename std::enable_if<
		std::is_same<
		decltype(std::declval<C>().reserve(std::declval<typename C::size_type>())),
		void>::value>::type > : std::true_type {};
}