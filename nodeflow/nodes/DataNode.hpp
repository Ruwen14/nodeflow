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
#include <optional>

#include "nodeflow/core/Node.hpp"
#include "nodeflow/nodes/FlowNode.hpp"

namespace nf
{
template <typename Type>
class DataNodeImpl;

class DataNode : public Node
{
public:
    DataNode() = default;

    std::string nodeName() const override;

    NodeArchetype getArchetype() const final;

    virtual std::unique_ptr<FlowNode> spawnSetter() const = 0;

public:
    template <typename T>
    bool setValue(T&& data)
    {
        // ToDo: Maybe change to static cast on release
        if (auto impl = dynamic_cast<DataNodeImpl<T>*>(this))
        {
            impl->m_data.value = std::forward<T>(data);
            return true;
        }
        NF_ASSERT(false,
                  "Node does not hold data of type 'T' explicitly. Try calling setValue<T>(...) "
                  "for implicit conversion.");
        return false;
    }
};

template <typename Type>
class DataSetterNode : public FlowNode
{
public:
    DataSetterNode()
    {
        // Reserve exact amount of memory we need in port vector
        allocateExpectedPortCount(PortDirection::Input, 2);
        allocateExpectedPortCount(PortDirection::Output, 0);
    }

    std::string nodeName() const override
    {
        return std::string(type_name<DataSetterNode<Type>>());
    }

    ErrorOr<void> setup() override
    {
        addPort(m_dataNodeData);
        addPort(m_setValue);

        return {};
    }

    typeid_t nodeType() const override
    {
        return type_id<DataSetterNode<Type>>();
    }

    void process() override
    {
        auto dataNodeData = getInputMutable(m_dataNodeData);
        auto setValue = getInput(m_setValue);

        if (dataNodeData && setValue)
            *dataNodeData = *setValue;
    }

    NodeArchetype getArchetype() const override
    {
        return NodeArchetype::FlowNode;
    }

private:
    InputPort<Type> m_dataNodeData;
    InputPort<Type> m_setValue;
};

template <typename Type>
class DataNodeImpl : public DataNode
{
    friend class DataNode;

public:
    static std::string staticNodeName;

public:
    DataNodeImpl()
    {
        // Reserve exact amount of memory we need in port vector
        allocateExpectedPortCount(PortDirection::Input, 0);
        allocateExpectedPortCount(PortDirection::Output, 1);
    }
    DataNodeImpl(Type&& value)
    {
        m_data.setValue(std::forward<Type>(value));
        // Reserve exact amount of memory we need in port vector
        allocateExpectedPortCount(PortDirection::Input, 0);
        allocateExpectedPortCount(PortDirection::Output, 1);
    }

    ErrorOr<void> setup() override
    {
        addPort(m_data);
        return {};
    }

    std::string nodeName() const override
    {
        return std::string(type_name<DataNodeImpl<Type>>());
    }

    typeid_t nodeType() const override
    {
        return type_id<DataNodeImpl<Type>>();
    }

    bool streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive) final
    {
        if (index != 0)
            return false;
        if (flag == StreamFlag::WriteTo)
            return m_data.serialize(archive);
        return m_data.deserialize(archive);
    }

    std::optional<std::string> getOutputAsString(PortIndex index) override
    {
        if (index != 0)
            return std::nullopt;

        return m_data.getValueAsString();
    }

    bool setOutputFromString(PortIndex index, const std::string& val) override
    {
        if (index != 0)
            return false;

        return m_data.setValueFromString(val);
    }

    std::unique_ptr<FlowNode> spawnSetter() const final
    {
        return std::make_unique<DataSetterNode<Type>>();
    }

private:
    OutputPort<Type> m_data;
};

//     template <typename T>
//     static const std::optional<T> getData(DataNode& node);

// template <typename T>
// const std::optional<T> DataNode::getData(DataNode& node)
// {
//     static constexpr typeid_t getDataTypeID = type_id<T>();
//     if (node.underlyingDataTypeID() != getDataTypeID)
//         return std::nullopt;
//
//     NF_ASSERT(dynamic_cast<VariableNodeImpl<T>*>(&node) != nullptr,
//               "Oops, this shoudln't happen. type_id<T>() might have a hash "
//               "collision?");
//     auto& actualNode = static_cast<VariableNodeImpl<T>&>(node);
//     return actualNode.data();
// }
} // namespace nf