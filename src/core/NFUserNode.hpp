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
#include "core/NFAbstractNodeModel.hpp"

#include <tuple>
#include <type_traits>
#include <array>
#include <string>

namespace nodeflow
{
	template<int Count, class... Types>
	struct InputPins
	{
		static_assert(Count == sizeof...(Types), "Error setting InputPins: 'PinCount' and 'PinTypes' of unequal length");
		using types = std::tuple<Types...>;
		static constexpr int count = Count;
	};

	template<int Count, class... Types>
	struct OutputPins
	{
		static_assert(Count == sizeof...(Types), "Error setting OutputPins: 'PinCount' and 'PinTypes' of unequal length");
		using types = std::tuple<Types...>;
		static constexpr int count = Count;
	};

	template<typename InputPins_, typename OutputPins_>
	class NFUserNode 
	{
	public:
		using inputTypes = InputPins_::types;
		using outputTypes = OutputPins_::types;
		template<std::size_t PinIndex>
		using mappedInTyp = std::tuple_element_t<PinIndex, inputTypes>;
		template<std::size_t PinIndex>
		using mappedOutTyp = std::tuple_element_t<PinIndex, outputTypes>;

		static constexpr int inputCount = InputPins_::count;
		static constexpr int outputCount = OutputPins_::count;

	public:
		NFUserNode() = default;
		NFUserNode(const std::string& nodeCaption,
			const std::array<std::string, inputCount>& inNames,
			const std::array<std::string, outputCount>& outNames
		);

	protected:
		template<std::size_t PinIndex>
		auto getInput() const
		{
			static_assert(PinIndex < inputCount, "'PinIndex' out of bounds.");
			using RType = std::tuple_element_t<PinIndex, inputTypes>;
			RType a;
			return a;
		}

		template<std::size_t PinIndex>
		auto setOutput(const mappedOutTyp<PinIndex>& val)
		{
			print(typeid(val).name());
			using SType = std::tuple_element_t<PinIndex, outputTypes>;
			static_assert(std::is_same_v< mappedOutTyp<PinIndex>, SType>, "Alarm");
			static_assert(PinIndex < outputCount, "'PinIndex' out of bounds");

			return val;
		}

	public:
		template<int PinIndex>
		std::string outputName() const noexcept
		{
			static_assert(PinIndex <= outputCount, "'PinIndex' out of bounds");
			return "";
		}

		template<int PinIndex>
		std::string inputName() const noexcept
		{
			static_assert(PinIndex <= inputCount, "'PinIndex' out of bounds");
			return "";
		}

		void setInputNames(const std::array<std::string, inputCount>& names) noexcept
		{
		}

		void setOutputNames(const std::array<std::string, outputCount>& names) noexcept
		{
		}

		void setNodeCaption(const std::string& caption) noexcept
		{
		}
	};
}
