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
#include <type_traits>
#include <iostream>
#include <ostream>
#include <istream>
#include <functional>


namespace nf
{
	template <class T, class... Types>
	inline constexpr bool is_any_of_v = std::disjunction<std::is_same<T, Types>...>{};

	template< size_t I, typename T, typename Tuple_t>
	constexpr size_t index_in_tuple_fn() {
		static_assert(I < std::tuple_size<Tuple_t>::value, "The element is not in the tuple");

		using el = typename std::tuple_element<I, Tuple_t>::type;
		if constexpr (std::is_same<T, el>::value) {
			return I;
		}
		else {
			return index_in_tuple_fn<I + 1, T, Tuple_t>();
		}
	}

	template<typename T, typename Tuple_t>
	struct index_in_tuple {
		static constexpr size_t value = index_in_tuple_fn<0, T, Tuple_t>();
	};

	template<typename T, typename Tuple_t>
	struct extract_type
	{
		static constexpr size_t value = index_in_tuple_fn<0, T, Tuple_t>();
		using type = std::tuple_element_t<value, Tuple_t>;
	};


	template<class... Tuples >
	struct tuple_concat
	{
		using value = decltype(std::tuple_cat(std::declval<Tuples>() ...));
	};

	template<typename S, typename T>
	struct is_streamable
	{
		template<typename SS, typename TT>
		static auto test(int)
			-> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

		template<typename, typename>
		static auto test(...)->std::false_type;

		static constexpr auto value = decltype(test<S, T>(0))::value;
	};

	template<typename T>
	static constexpr auto has_ostream_operator_v = is_streamable<std::ostream, T>::value;

	template<typename S, typename T>
	struct is_istreamable
	{
		template<typename SS, typename TT>
		static auto test(int)
			-> decltype(std::declval<SS&>() >> std::declval<TT&>(), std::true_type());

		template<typename, typename>
		static auto test(...) -> std::false_type;

		static constexpr auto value = decltype(test<S, T>(0))::value;
	};

	template<typename T>
	static constexpr auto has_istream_operator_v = is_istreamable<std::istream, T>::value;



	template<typename T>
	struct deduce_class_type;

	template<typename Class, typename Value>
	struct deduce_class_type<Value Class::*>
	{
		using type = Class;
	};

	template<typename T>
	struct deduce_member_type;

	template<typename Class, typename Value>
	struct deduce_member_type<Value Class::*>
	{
		using type = Value;
	};

	template<typename Class>
	using deduce_member_type_t = typename deduce_member_type<Class>::type;



	template<typename Ret, class... Params>
	struct FuncSignature
	{
	};

	template<typename Ret, class... Params>
	struct FuncSignature<std::function<Ret(Params...)>>
	{
		using ReturnType_t = Ret;
		using ParamTypes_t = std::tuple<Params...>;
	};

// 
	template<typename Ret, typename Cls, class... Params >
	struct FuncSignature<std::function<Ret (Cls::*)(Params...)>>
	{
		using ReturnType_t = Ret;
		using ParamTypes_t = std::tuple<Params...>;
		using ClassType_t = Cls;
	};

}
