#pragma once
#include <iostream>
#include "utility/entt.h"

template<typename T>
inline void pprint_internal(const T& t)
{
	std::cout << t;
}

namespace nf
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
			print(args...); // pass the args further
		}
	}


	void printIntId()
	{
		print(entt::type_id<std::string>().index());
	}
}

