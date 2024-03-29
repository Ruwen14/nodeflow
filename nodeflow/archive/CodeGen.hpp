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

#include "core/type_tricks.hpp"
#include "../3rdparty/entt/single_include/entt/entt.hpp"

namespace nf
{

	template< typename T >
	struct type_names {};

	template< typename... Ts >
	struct type_names< std::tuple< Ts... > >
	{
		static constexpr std::array<std::string_view, sizeof...(Ts)> value = { {entt::type_name<Ts>::value()...} };
	};

	template<auto Func>
	struct FunctionCodeContext
	{
		using ReturnType = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ReturnType_t;
		using ArgumentTypes = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ParamTypes_t;

		static constexpr auto FunctionName() noexcept
		{
			std::string_view pretty_function{ ENTT_PRETTY_FUNCTION };
			auto first = pretty_function.find_first_not_of(' ', pretty_function.find_first_of(ENTT_PRETTY_FUNCTION_PREFIX) + 1);
			auto value = pretty_function.substr(first, pretty_function.find_last_of(ENTT_PRETTY_FUNCTION_SUFFIX) - first);
			auto valuebefore = value.substr(0, value.find_first_of('('));
			auto valuelast = valuebefore.substr(valuebefore.find_last_of(' ') + 1);
			return valuelast;
		}

		static constexpr auto ReturnTypeName() noexcept
		{
			return entt::type_name<ReturnType>::value();
		}

		static constexpr auto ArgumentCount() noexcept
		{
			return std::tuple_size_v<ArgumentTypes>;
		}

		static constexpr auto ArgumentTypeNames() noexcept
		{
			return type_names<ArgumentTypes>::value;
		}
	};


}


template<uint32_t hash>
struct HeaderDependencyContext
{};

#define HASH_STRING(String_)  entt::hashed_string::value(String_)

#define NF_NATIVIZE_FUNC(Func) \
	template<> \
	struct HeaderDependencyContext<entt::hashed_string::value(#Func)> \
	{ \
		static constexpr auto headerfile = __FILE__;  \
		static_assert(std::string_view(headerfile).ends_with(".hpp") || std::string_view(headerfile).ends_with(".cpp"),  \
		"NF_NATIVIZE_FUNC(...) can only be placed within declaration files (.h or .hpp)"); \
	}; \

