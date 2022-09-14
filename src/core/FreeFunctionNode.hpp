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


#include "IOPort.h"
#include "TypeTricks.hpp"
#include <tuple>
#include <type_traits>

namespace nf
{

	template<class... Ts>
	struct MakeInputPorts
	{
		using value = std::tuple<nf::InputPort<Ts>...>;
	};

	template<class... Ts>
	struct MakeInputPorts < std::tuple<Ts...>>
	{
		using value = std::tuple<nf::InputPort<Ts>...>;
	};

	template<auto Func>
	class FreeFunctionNode
	{
	public:
		using ReturnType_t = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ReturnType_t;
		using ArgumentTypes_t = typename nf::FuncSignature<decltype(std::function{ Func }) > ::ParamTypes_t;
		using InputPorts_t = MakeInputPorts<ArgumentTypes_t>::value;

	public:
		static constexpr auto hasInputs = std::tuple_size_v<InputPorts_t> != 0;
		static constexpr auto hasOutputs = !std::is_void_v<ReturnType_t>;

	public:
		void process(ArgumentTypes_t t)
		{
			if constexpr (hasOutputs)
			{
				oport.value = std::apply(Func, iports);
			}
			{
				std::apply(Func, t);
			}
		}

	public:
		InputPorts_t iports;
		nf::OutputPort<ReturnType_t> oport;

	};

}


template<typename T>
struct wrapper
{
public:
	T val{};
};

template<typename T>
constexpr T wrapper_get(const wrapper<T>& w)
{
	return w.val;
}


namespace nf
{
	// Implementation taken from std::apply MSVC
	// Compare to https://en.cppreference.com/w/cpp/utility/apply - Implementation

	template <class Func, class Tuple, size_t... seq>
	constexpr decltype(auto) ApplyPinsOnCallable_Impl(Func&& callable, Tuple&& tupl, std::index_sequence<seq...>)
	{
		return std::invoke(std::forward<Func>(callable),
			wrapper_get(std::get<seq>(std::forward<Tuple>(tupl)))...);
	}

	template <class Func, class Tuple>
	constexpr decltype(auto) ApplyPinsOnCallable(Func&& callable, Tuple&& tpl)
	{
		return ApplyPinsOnCallable_Impl(std::forward<Func>(callable), std::forward<Tuple>(tpl),
			std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
	}

}