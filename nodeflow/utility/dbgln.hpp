/*
- nodeflow -
BSD 3-Clause License

Copyright (c) 2022, Ruwen Kohm
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <format>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <optional>
#include <utility>
#include <map>
#include <unordered_map>
#include <sstream>
#include <type_traits>
#include <iostream>


#include "../3rdparty/cpputils/magic_enum.h"


template<typename... Args>
void dbgln(std::string_view fmt, Args&&... args)
{
	std::cout << std::vformat(fmt, std::make_format_args(args...)) << "\n";
}

template<typename T>
void dbgln(T&& t)
{
	dbgln("{}", std::forward<T>(t));
}

template<typename... Args>
std::string outln(std::string_view fmt, Args&&... args)
{
	return std::vformat(fmt, std::make_format_args(args...));
}

template<typename T>
std::string outln(T&& t)
{
	return outln("{}", std::forward<T>(t));
}


template<typename T, class CharT>
struct std::formatter<std::vector<T>, CharT> : std::formatter<T, CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::vector<T>& cont, FormatContext& fc) const {
		std::format_to(fc.out(), "{}", "[");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			if (it != cont.begin())
				std::format_to(fc.out(), ", ");
			std::format_to(fc.out(), "{}", *it);
		}
		return std::format_to(fc.out(), "{}", "]");
	}
};

template<typename T, class CharT>
struct std::formatter<std::list<T>, CharT> : std::formatter<T, CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::list<T>& cont, FormatContext& fc) const {
		std::format_to(fc.out(), "{}", "[");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			if (it != cont.begin())
				std::format_to(fc.out(), ", ");
			std::format_to(fc.out(), "{}", *it);
		}
		return std::format_to(fc.out(), "{}", "]");
	}
};

template<typename T, class CharT>
struct std::formatter<std::set<T>, CharT> : std::formatter<T, CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::set<T>& cont, FormatContext& fc) const {
		std::format_to(fc.out(), "{}", "[");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			if (it != cont.begin())
				std::format_to(fc.out(), ", ");
			std::format_to(fc.out(), "{}", *it);
		}
		return std::format_to(fc.out(), "{}", "]");
	}
};

template<typename E>
concept EnumType = std::is_enum_v<E>;

template<EnumType Enum, class CharT>
struct std::formatter<Enum, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const Enum enum_value, FormatContext& fc) const {
		std::string out;
		auto enum_value_str = magic_enum::enum_name(enum_value);
		auto enum_type_str = magic_enum::enum_type_name<Enum>();
		out += enum_type_str;
		out += "::";
		out += enum_value_str;
		return std::format_to(fc.out(), "{}", out);
	}
};

template<typename T, class CharT>
struct std::formatter<T*, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(T* ptr, FormatContext& fc) const {
		if (!ptr)
			return std::format_to(fc.out(), "{}", "nullptr");

		std::ostringstream address;
		address << static_cast<void*>(ptr);
		return std::format_to(fc.out(), "{}", address.str());
	}
};

template<typename T, class CharT>
struct std::formatter<std::optional<T>, CharT> : std::formatter<T, CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::optional<T>& opt, FormatContext& fc) const {
		if (opt)
			return std::format_to(fc.out(), "{}", opt.value());
		return std::format_to(fc.out(), "{}", "nullopt");
	}
};

template<typename T, typename T2, class CharT>
struct std::formatter<std::pair<T,T2>, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::pair<T, T2>& pr, FormatContext& fc) const {
		return std::format_to(fc.out(), "({}, {})", pr.first, pr.second);
	}
};

template<typename T, typename T2, class CharT>
struct std::formatter<std::map<T, T2>, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::map<T, T2>& m, FormatContext& fc) const {
		std::format_to(fc.out(), "{}", "{\n");
		for (const auto& [k, v] : m)
		{
			std::format_to(fc.out(), "\t{} : {}", k, v);
			std::format_to(fc.out(), "{}", "\n");

		}
		return std::format_to(fc.out(), "{}", '}');

	}
};

template<typename T, typename T2, class CharT>
struct std::formatter<std::unordered_map<T, T2>, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const std::unordered_map<T, T2>& m, FormatContext& fc) const {
		std::format_to(fc.out(), "{}", "{\n");
		for (const auto& [k, v] : m)
		{
			std::format_to(fc.out(), "\t{} : {}", k, v);
			std::format_to(fc.out(), "{}", "\n");

		}
		return std::format_to(fc.out(), "{}", '}');

	}
};



