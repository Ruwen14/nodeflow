#pragma once
#include <iostream>

template<typename T>
inline void pprint_internal(const T& t)
{
	std::cout << t;
}

namespace nodeflow
{
	template <typename Arg, typename...Args>
	inline constexpr void print(const Arg& arg, const Args&... args) noexcept
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
}