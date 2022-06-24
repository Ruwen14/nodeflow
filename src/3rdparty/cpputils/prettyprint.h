#pragma once
#include "typecheck.h"
#include <iostream>
#include "stringutils.h"
#include <string>

namespace cpputils
{
	std::string demangle(const char* name) {
#ifdef __GNUG__
		int status = -4;
		std::unique_ptr<char, void(*)(void*)> res{
		  abi::__cxa_demangle(name, NULL, NULL, &status),
		  std::free
		};
		return (status == 0) ? res.get() : name;
#else
		return name;
#endif
	}

	template <typename T>
	std::string type(const T& t) {
		return demangle(typeid(t).name());
	}

	template<typename T, typename = typename
		std::enable_if<std::is_fundamental<T>::value>::type>
	inline void pprint_internal(const T t)
	{
		std::cout << t;
	}
	template<typename T, typename = typename
	std::enable_if<!std::is_fundamental<T>::value>::type>
	inline void pprint_internal(const T& t)
	{
		std::cout << stringify(t);
	}

	inline void pprint_internal(const bool b)
	{
		std::cout << (b ? "true" : "false");
	}

	inline void pprint_internal(const std::string& s)
	{
		std::cout << s;
	}

	inline void pprint_internal(const char* s)
	{
		std::cout << s;
	}

	inline void pprint_internal(const std::string_view s)
	{
		std::cout << s;
	}

	template <typename Arg, typename...Args>
	inline constexpr void pprint(const Arg& arg, const Args&... args) noexcept
	{
		if constexpr (sizeof...(Args) == 0)
		{
			pprint_internal(arg);
			std::cout << "\n";
		}
		else
		{
			pprint_internal(arg);
			std::cout << ", ";
			pprint(args...); // pass the args further
		}
	}

	template<typename T>
	inline void pprint_type(const T& t)
	{
		pprint(type(t));
	}
}
