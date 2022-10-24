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
#include <tuple>



#include "3rdparty/entt/single_include/entt/entt.hpp"

namespace nf::detail
{
	template<class ITs = std::tuple<void>, class OTs = std::tuple<void>>
	struct StaticNodeArgs_
	{
		using InputPortTypes = ITs;
		using OutputPortTypes = OTs;
		static constexpr int CountInPorts = std::tuple_size_v<InputPortTypes>;
		static constexpr int CountOutPorts = std::tuple_size_v<OutputPortTypes>;

		template<class... InTs>
		using SetupInput = StaticNodeArgs_<std::tuple<InTs...>, OTs>;

		template<class... OutTs>
		using SetupOutput = StaticNodeArgs_<ITs, std::tuple<OutTs...>>;

	};
}

namespace nf::detail
{
	template<
		class FixedITs,
		class FixedOTs,
		class DynITs,
		class DynOTs,
		int maxI,
		int maxO >
		struct DynamicNodeArgs_
	{

		using InputPortTypes = FixedITs;
		static constexpr int CountInPorts = std::tuple_size_v<FixedITs>;

		template<class... InTs>
		using SetupFixedInput = DynamicNodeArgs_<std::tuple<InTs...>, FixedOTs, DynITs, DynOTs, maxI, maxO>;

		template<class... OutTs>
		using SetupFixedOutput = DynamicNodeArgs_<FixedITs, std::tuple<OutTs...>, DynITs, DynOTs, maxI, maxO>;

		template<class... DynInTs>
		using SetupDynamicInput = DynamicNodeArgs_<FixedITs, FixedOTs, std::tuple<DynInTs...>, DynOTs, maxI, maxO>;

		template<class... DynOutTs>
		using SetupDynamicOutput = DynamicNodeArgs_<FixedITs, FixedOTs, DynITs, std::tuple<DynOutTs...>, maxI, maxO>;
	};




}

using StaticNodeArgs = nf::detail::StaticNodeArgs_<>;
using DynamicNodeArgs = nf::detail::DynamicNodeArgs_<std::tuple<void>, std::tuple<void>, std::tuple<void>, std::tuple<void>, 0, 0>;

using Args = StaticNodeArgs::SetupInput<int>;


using Args2 = DynamicNodeArgs
::SetupFixedInput<int, float>
::SetupDynamicInput<int>
::SetupFixedOutput<char>
::SetupDynamicOutput<bool>;


template<class... Ts>
struct TypeHashArray
{
	static constexpr std::array<uint32_t, sizeof...(Ts)> value = { {entt::type_hash<Ts>::value()...} };

};

template<class... Ts>
struct TypeHashArray<std::tuple<Ts...>>
{
	static constexpr std::array<uint32_t, sizeof...(Ts)> value = { {entt::type_hash<Ts>::value()...} };

};






















namespace nf
{
	template<class... Types>
	struct PinTypes
	{
		using types = std::tuple<Types...>;
		static constexpr int count = sizeof...(Types);
	};
	
	template<class... Types>
	using In = PinTypes<Types...>;
	
	template<class... Types>
	using Out = PinTypes<Types...>;
	
	template<class InTypePack, class OutTypePack>
	class StaticNodeModel;
	
	template<class... InTs, class... OutTs>
	class StaticNodeModel<In<InTs...>, Out<OutTs...>>
	{
	public:
		using TypesInPorts = In<InTs...>::types;
		using TypesOutPorts = Out<OutTs...>::types;
		static constexpr int CountInPorts = In<InTs...>::count;
		static constexpr int CountOutPorts = Out<OutTs...>::count;
	};

}


template<class... InTs>
struct Input {
	template<class... OutTs>
	struct Output {
		using InTypes = std::tuple<InTs...>;
		static constexpr int InCount = sizeof...(InTs);

		using OutTypes = std::tuple<OutTs...>;
		static constexpr int OutCount = sizeof...(OutTs);
	};
};


class UserNode : public StaticNodeModel<In<int, double>, Out<bool>>
{
public:
	
};

