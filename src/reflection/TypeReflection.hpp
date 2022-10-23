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

#include <cstdint>
#include <string_view>

#define NF_FUNC_SIGNATURE __FUNCSIG__
#define NF_FUNC_SIGNATURE_PREFIX '<'
#define NF_FUNC_SIGNATURE_SUFFIX '>'

#ifndef NF_TYPE_ID
	#define NF_TYPE_ID std::uint64_t
#endif // !NF_TYPE_ID


namespace nf
{
	namespace detail
	{
		template<typename>
		struct Fnv1aConfig;

		template<>
		struct Fnv1aConfig<std::uint32_t>
		{
			using type_t = std::uint32_t;
			static constexpr std::uint32_t offset = 2166136261;
			static constexpr std::uint32_t prime = 16777619;
		};

		template<>
		struct Fnv1aConfig<std::uint64_t>
		{
			using type_t = std::uint64_t;
			static constexpr std::uint64_t offset = 14695981039346656037ull;
			static constexpr std::uint64_t prime = 1099511628211ull;
		};

		template<typename HashType>
		struct Fnv1aHasher
		{
			using hashConfig = Fnv1aConfig<HashType>;
			static constexpr auto hash(const char* str, const std::size_t len) noexcept
			{
				auto hashVal = hashConfig::offset;
				for (std::size_t pos = 0; pos < len; ++pos)
				{
					hashVal = (hashVal ^ static_cast<hashConfig::type_t>(str[pos])) * hashConfig::prime;
				}
				return hashVal;
			}

		};
	}

	using typeid_t = NF_TYPE_ID;

	template<typename Type>
	constexpr auto typeName() noexcept
	{
		constexpr std::string_view pretty_function{ NF_FUNC_SIGNATURE };
		constexpr auto first = pretty_function.find_first_not_of(' ', pretty_function.find_first_of(NF_FUNC_SIGNATURE_PREFIX) + 1);
		constexpr auto value = pretty_function.substr(first, pretty_function.find_last_of(NF_FUNC_SIGNATURE_SUFFIX) - first);
		return value;
	}


	template<typename Type>
	constexpr nf::typeid_t typeID() noexcept
	{
		constexpr auto tName = typeName<Type>();
		return detail::Fnv1aHasher<nf::typeid_t>::hash(tName.data(), tName.size());
	}

	template<typename Type>
	struct refltype
	{
		using type_t = Type;

		static constexpr std::string_view name() noexcept
		{
			return typeName<Type>();
		}

		static constexpr typeid_t id() noexcept
		{
			return typeID<Type>();
		}
	};

	template<typename... Types>
	struct refltypes
	{
		using types_t = std::tuple<Types...>;

		static constexpr auto names() noexcept
		{
			return std::array<std::string_view, sizeof...(Types)>{ {nf::typeName<Types>()...} };
		}

		static constexpr auto ids() noexcept
		{
			return std::array<typeid_t, sizeof...(Types)>{ {nf::typeID<Types>()...}};
		}

	};


	template<typename... Types>
	struct refltypes<std::tuple<Types...>>
	{
		using types_t = std::tuple<Types...>;

		static constexpr auto names() noexcept
		{
			return std::array<std::string_view, sizeof...(Types)>{ {nf::typeName<Types>()...} };
		}

		static constexpr auto ids() noexcept
		{
			return std::array<typeid_t, sizeof...(Types)>{ {nf::typeID<Types>()...}};
		}
	};

// 	template< typename T >
// 	struct type_names;
// 
// 	template< typename... Ts >
// 	struct type_names< std::tuple< Ts... > >
// 	{
// 		static constexpr std::array<std::string_view, sizeof...(Ts)> value = { {entt::type_name<Ts>::value()...} };
// 	};





}