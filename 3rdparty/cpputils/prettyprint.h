#pragma once
#include "typecheck.h"
#include <iostream>
#include "stringutils.h"
#include <string>

namespace cpputils
{
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


	template<typename T>
	inline void pprint_internal(T* ptr)
	{
		if (ptr)
			std::cout << ptr;
		else
			std::cout << "nullptr";
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
