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

#include "../3rdparty/entt/single_include/entt/entt.hpp"
#include "TypeTricks.hpp"

#include <tuple>
#include <string_view>
#include <type_traits>
#include <format>



namespace nf
{
	namespace internal
	{
		template< typename T >
		struct type_names;

		template< typename... Ts >
		struct type_names< std::tuple< Ts... > >
		{
			static constexpr std::array<std::string_view, sizeof...(Ts)> value = { {entt::type_name<Ts>::value()...} };
		};





		template<auto Func>
		struct FreeFunctionReflection
		{
			using ReturnType_t = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ReturnType_t;
			using ArgumentTypes_t = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ParamTypes_t;
			static constexpr auto isMemberFunction = false;


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
				return entt::type_name<ReturnType_t>::value();
			}

			static constexpr auto ArgumentTypeNames() noexcept
			{
				return internal::type_names<ArgumentTypes_t>::value;
			}

			static constexpr auto ArgumentCount()
			{
				return std::tuple_size_v<ArgumentTypes_t>;
			}

		};

		template<auto Func>
		struct MemberFunctionReflection
		{
			using Signature_t = nf::FuncSignature<std::function<decltype(Func)>>;
			using ReturnType_t = Signature_t::ReturnType_t;
			using ArgumentTypes_t = Signature_t::ParamTypes_t;
			using ClassType_t = Signature_t::ClassType_t;

			static constexpr auto isMemberFunction = true;

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
				return entt::type_name<ReturnType_t>::value();
			}

			static constexpr auto ArgumentTypeNames() noexcept
			{
				return internal::type_names<ArgumentTypes_t>::value;
			}

			static constexpr auto ClassTypeName() noexcept
			{
				return entt::type_name<ClassType_t>::value();
			}

			static constexpr auto ArgumentCount()
			{
				return std::tuple_size_v<ArgumentTypes_t>;
			}

		};


		template<typename Class>
		struct ClassReflection
		{

		};

		template<typename Type>
		struct TypeReflection
		{
			static constexpr auto Name() noexcept
			{
				return entt::type_name<Type>::value();
			}
		};


	}


	struct Reflection
	{
		template<auto Func>
		using Function = std::conditional_t<std::is_member_function_pointer_v<decltype(Func)>, internal::MemberFunctionReflection<Func>, internal::FreeFunctionReflection<Func>>;

		template<typename Class_>
		using Class = Class_;

		template<typename Type_>
		using Type = internal::TypeReflection<Type_>;
	};


	template<auto Func>
	std::string constructFunctionCall()
	{
		using ReflectedFunction = nf::Reflection::Function<Func>;

		auto named_args = nf::Reflection::Function<Func>::ArgumentTypeNames();

		return std::format("{} __LocalVar__{} = {}({})",
			ReflectedFunction::ReturnTypeName(),
			__COUNTER__, ReflectedFunction::FunctionName(), cpputils::str_join(named_args.begin(), named_args.end(), ", "));


	}

}