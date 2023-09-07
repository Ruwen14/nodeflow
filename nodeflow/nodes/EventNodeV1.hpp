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

#include "typedefs.hpp"
#include "core/type_tricks.hpp"
#include "nodes/FlowNode.hpp"

namespace nf {
	template<class EventType, class... Fields>
	class EventNodeV1 {};

	template<class Event, class... Fields>
	class EventNodeV1<Event, std::tuple<Fields...>> : public FlowNode
	{
	public:
		using OutputTypes_t = std::tuple<Fields...>;
		using OutputPorts_t = ExpandOutputPorts<OutputTypes_t>::value;
		static constexpr bool hasFields = std::tuple_size_v<OutputPorts_t> != 0;

		ErrorOr<void> setup() override
		{
			if constexpr (hasFields)
				std::apply([this](auto&... port) { (this->addPort(port), ...); }, m_eventFields);
			return {};
		}

		NodeArchetype getArchetype() const override
		{
			return NodeArchetype::Flow_EventNode;
		}

		// Visitor Pattern
		// Or Make template<Event> class BaseEventNode with virtual template<typename Event> parseEvent
		bool constructFromEvent(const Event& event /*We need a callback function here*/)
		{
			return false;
		}

		bool setFieldNames(const std::vector<std::string_view>& fieldNames)
		{
			if constexpr (hasFields)
			{
				if (fieldNames.size() < m_outputPorts.size())
				{
					NF_ASSERT(false, "Event has more fields than provided fieldNames");
					return false;
				}

				for (size_t i = 0; i < m_outputPorts.size(); i++)
				{
					auto& oPort = m_outputPorts[i];
					oPort.setName(std::string(fieldNames[i]));
				}

				return true;
			}
			else
				return false;
		}

		const std::vector<std::string>& fieldNames() const
		{
			return {};
		}

	public:
		OutputPorts_t m_eventFields;
	};
}