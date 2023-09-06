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
#include <vector>
#include <string>
#include <format>

#include "typedefs.hpp"
#include "core/Node.hpp"
#include "nodes/FlowNode.hpp"
#include "core/type_tricks.hpp"

namespace nf
{
	template<auto Func>
	class FunctorNode : public FlowNode
	{
	public:
		static std::string staticNodeName;
		static std::string staticResultPortName;
		static std::vector<std::string> staticArgPortNames;

	public:
		using FSig_t = typename FuncSignature<decltype(std::function{ Func }) > ;
		using FReturn_t = FSig_t::ReturnType_t;
		using FArgument_ts = FSig_t::ParamTypes_t;
		using InputPorts_t = ExpandInputPorts<FArgument_ts>::value;

		static constexpr bool hasInputs = std::tuple_size_v<InputPorts_t> != 0;
		static constexpr bool hasOutput = !std::is_void_v<FReturn_t>;

	public:

		std::string nodeName() const override
		{
			return staticNodeName;
		}

		NodeArchetype getArchetype() const final
		{
			return NodeArchetype::Flow_FunctorNode;
		}

		std::string portName(PortDirection dir, PortIndex index) const override
		{
			if (dir == PortDirection::Input)
			{
				NF_ASSERT(index < m_inputPorts.size(), "Port index out of range");
				if (!(index < staticArgPortNames.size()))
					return "";
				return staticArgPortNames.at(index);
			}

			if (dir == PortDirection::Output)
			{
				NF_ASSERT(index < m_outputPorts.size(), "Port index out of range");
				return staticResultPortName;
			}
			return "";
		}

		bool streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive) override
		{
			if (index != 0)
				return false;

			if (flag == StreamFlag::WriteTo)
				return m_resultPort.serialize(archive);
			return m_resultPort.deserialize(archive);
		}

		Expected<void, Error> setup() override
		{
			if constexpr (hasInputs)
				std::apply([this](auto&... port) { (this->addPort(port), ...); }, m_argumentPorts);

			if constexpr (hasOutput)
				addPort(m_resultPort);

			return {};
		}

		template<typename T>
		auto getInputStuff(nf::InputPort<T>& port)
		{
			int d = 4;
			int a = port.index() - +7 + d;
			NF_UNUSED(port);
			return a;
		}

		template <class Func, class Tuple, size_t... seq>
		constexpr decltype(auto) applyPortsOnCallable_Impl(Func&& callable, Tuple&& tupl, std::index_sequence<seq...>)
		{
			return std::invoke(std::forward<Func>(callable),
				*getInputData(std::get<seq>(std::forward<Tuple>(tupl)))...);
		}

		template <class Func, class Tuple>
		constexpr decltype(auto) applyPortsOnCallable(Func&& callable, Tuple&& tupl)
		{
			return applyPortsOnCallable_Impl(std::forward<Func>(callable), std::forward<Tuple>(tupl),
				std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
		}

		Expected<void, Error> onBuild() override
		{
			if constexpr (hasInputs)
			{
				for (const auto& iPort : m_inputPorts)
				{
					if (!iPort.link().valid())
					{
						return make_unexpected(
							Error(std::format("Build failed for Node '{}': one or more InputPort(s) not connected ",
								nodeName()), 120
							)
						);
					}
				}
			}
			return {};
		}

		void process() override
		{
			// Move to onCompile() / inBuild() override to save performance;
			for (const auto& port : m_inputPorts)
				NF_ASSERT(port.link().valid(), "InputPort not connected");

			if constexpr (hasInputs && hasOutput)
				m_resultPort.value = applyPortsOnCallable(Func, m_argumentPorts);

			else if constexpr (hasInputs && !hasOutput)
				applyPortsOnCallable(Func, m_argumentPorts);

			else if constexpr (!hasInputs && hasOutput)
				m_resultPort.value = Func();

			else
				Func();
		}

	public:
		InputPorts_t m_argumentPorts;
		OutputPort<FReturn_t> m_resultPort;
	};

	template<auto Func>
	std::string FunctorNode<Func>::staticNodeName = "FunctorNode";

	template<auto Func>
	std::string FunctorNode<Func>::staticResultPortName = "Result";

	template<auto Func>
	std::vector<std::string> FunctorNode<Func>::staticArgPortNames;
}