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

#include "typedefs.hpp"
#include "nodes/FlowNode.hpp"


namespace nf
{
	enum class ConversionPolicy
	{
		DontAddConversion,
		AddConversionIfNeeded
	};


	class ConversionNode : public FlowNode
	{
	public:
		std::string nodeName() const override;

		NodeArchetype getArchetype() const final;

		typeid_t from() const;
		typeid_t to() const;
	};

	template<typename FromType, typename ToType, auto ConversionCallable>
	class ConversionNodeImpl : public ConversionNode
	{
	public:
		static std::string staticNodeName;
		static std::string staticInputPortName;
		static std::string staticOutputPortName;

	public:
		ConversionNodeImpl();

		std::string nodeName() const override;

		std::string portName(PortDirection dir, PortIndex index) const override;

		Expected<void, Error> setup() override;

		bool streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive) final;

		void process() override
		{
			auto input = getInputData(m_fromPort);
			NF_ASSERT(input, "ConversionNodeImpl has no input connection");

			if (input)
				m_toPort.value = ConversionCallable(*input);
		}

	private:
		nf::InputPort<FromType> m_fromPort;
		nf::OutputPort<ToType> m_toPort;
	};

	template<typename FromType, typename ToType, auto ConversionCallable>
	std::string ConversionNodeImpl<FromType, ToType, ConversionCallable>::staticNodeName = "ConversionNodeImpl";

	template<typename FromType, typename ToType, auto ConversionCallable>
	std::string ConversionNodeImpl<FromType, ToType, ConversionCallable>::staticInputPortName = "Input";

	template<typename FromType, typename ToType, auto ConversionCallable>
	std::string ConversionNodeImpl<FromType, ToType, ConversionCallable>::staticOutputPortName = "Output";

	template<typename FromType, typename ToType, auto ConversionCallable>
	ConversionNodeImpl<FromType, ToType, ConversionCallable>::ConversionNodeImpl()
	{
		allocateExpectedPortCount(PortDirection::Input, 1);
		allocateExpectedPortCount(PortDirection::Output, 1);

	}

	template<typename FromType, typename ToType, auto ConversionCallable>
	std::string ConversionNodeImpl<FromType, ToType, ConversionCallable>::nodeName() const
	{
		return staticNodeName;
	}


	template<typename FromType, typename ToType, auto ConversionCallable>
	std::string ConversionNodeImpl<FromType, ToType, ConversionCallable>::portName(PortDirection dir, PortIndex index) const
	{
		NF_ASSERT(index == 0, "ConversionNodeImpl only has 1 input and 1 output");
		if (dir == PortDirection::Input)
			return staticInputPortName;
		return staticOutputPortName;
	}

	template<typename FromType, typename ToType, auto ConversionCallable>
	Expected<void, Error> ConversionNodeImpl<FromType, ToType, ConversionCallable>::setup()
	{
		addPort(m_fromPort);
		addPort(m_toPort);

		return {};
	}

	template<typename FromType, typename ToType, auto ConversionCallable>
	bool ConversionNodeImpl<FromType, ToType, ConversionCallable>::streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive)
	{
		if (index != 0)
			return false;
		if (flag == StreamFlag::WriteTo)
			return m_toPort.serialize(archive);
		return m_toPort.deserialize(archive);
	}

}



