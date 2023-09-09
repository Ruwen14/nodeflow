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

#include "nodeflow/core/type_tricks.hpp"
#include "nodeflow/nodes/FlowNode.hpp"
#include "nodeflow/typedefs.hpp"

namespace nf
{
class StartEventNode : public FlowNode
{
public:
    StartEventNode();

    std::string nodeName() const override;

    ErrorOr<void> setup() override;

    void process() override;

    NodeArchetype getArchetype() const override;
};

template <typename EventType>
class EventNode : public FlowNode
{
public:
    using Event_t = EventType;
    virtual void constructFromEvent(const EventType& event) = 0;

    NodeArchetype getArchetype() const override
    {
        return NodeArchetype::Flow_EventNode;
    }

    void process() override
    {
    }
};

template <typename EventType, auto... MemAccessers>
class GeneratedEventNode : public EventNode<EventType>
{
public:
    using fieldTypes = std::tuple<nf::deduce_member_type_t<decltype(MemAccessers)>...>;
    using OutputPorts_t = ExpandOutputPorts<fieldTypes>::value;
    static constexpr bool hasFields = std::tuple_size_v<OutputPorts_t> != 0;

    ErrorOr<void> setup() override
    {
        if constexpr (hasFields)
            std::apply([this](auto&... port) { (this->addPort(port), ...); }, m_eventFields);
        return {};
    }

    void constructFromEvent(const EventType& event) override
    {
        if constexpr (hasFields)
            std::apply(
                [&event](auto&&... port) {
                    ((port.value = std::invoke(MemAccessers, event)), ...);
                },
                m_eventFields);
    }

    bool setFieldNames(const std::vector<std::string_view>& fieldNames)
    {
        if constexpr (hasFields)
        {
            if (fieldNames.size() < this->m_outputPorts.size())
            {
                NF_ASSERT(false, "Event has more fields than provided fieldNames");
                return false;
            }

            for (size_t i = 0; i < this->m_outputPorts.size(); i++)
            {
                auto& oPort = this->m_outputPorts[i];
                oPort.setName(std::string(fieldNames[i]));
            }

            return true;
        }
        else
            return false;
    }

public:
    OutputPorts_t m_eventFields;
};
} // namespace nf