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
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <utility>

#include "nodeflow/core/Node.hpp"
#include "nodeflow/core/type_tricks.hpp"
#include "nodeflow/nodes/ConversionNode.hpp"
#include "nodeflow/nodes/DataNode.hpp"
#include "nodeflow/nodes/EventNode.hpp"
#include "nodeflow/nodes/FlowNode.hpp"
#include "nodeflow/nodes/FunctorNode.hpp"
#include "nodeflow/typedefs.hpp"
#include "nodeflow/utility/Expected.hpp"

namespace nf
{
struct lang
{
    static constexpr auto While = "WhileLoop";
    static constexpr auto IfElse = "IfElse";
    static constexpr auto For = "ForLoop";
};

class Node;
class FlowNode;

using VariableNode = DataNode;

enum class RegisterError
{
    EmptyNamePath,
    InvalidNamePath,
    NameAlreadyRegistered
};

struct InOutPortName
{
    std::string inputName = "Input";
    std::string outputName = "Output";
};

struct FunctorPortNames
{
    std::vector<std::string> inputNames;
    std::string outputName = "Output";
};

class FlowModule
{
public:
    using FlowNodeCreator = std::function<std::unique_ptr<nf::FlowNode>()>;
    using DataNodeCreator = std::function<std::unique_ptr<VariableNode>()>;

    using FlowNodeRegistry = std::map<std::string, FlowNodeCreator>;
    using DataNodeRegistry = std::map<std::string, DataNodeCreator>;

public:
    FlowModule() = default;
    explicit FlowModule(const std::string& moduleName)
        : m_moduleName(moduleName)
    {
    }

public:
    template <typename T>
    Expected<void, RegisterError> registerType(const std::string& namePath);

    template <class Node>
    Expected<void, RegisterError> registerCustomNode(const std::string& namePath);

    template <auto func>
    Expected<void, RegisterError> registerFunction(const std::string& namePath);

    template <auto Callable>
    Expected<void, RegisterError> registerConversion(const std::string& namePath,
                                                     const InOutPortName& portName = {});

    void setModuleName(const std::string& name);

    std::string moduleName() const;

    std::set<std::string> registered() const;

    const FlowNodeRegistry& nodeRegistry() const;

    const DataNodeRegistry& variableRegistry() const;

private:
    Expected<std::pair<std::string, std::string>, RegisterError> helperParseNamePath(
        const std::string& namePath) const;

public:
    std::string m_moduleName;
    FlowNodeRegistry m_flowNodeRegistry; // might use map for reduced memory consumption
    DataNodeRegistry m_dataNodeRegistry;
};

template <typename T>
Expected<void, RegisterError> FlowModule::registerType(const std::string& namePath)
{
    static_assert(!std::is_base_of<nf::Node, T>::value, "<T> must be a free Type");

    if (namePath.empty())
        return make_unexpected(RegisterError::EmptyNamePath);

    if (m_flowNodeRegistry.contains(namePath) || m_dataNodeRegistry.contains(namePath))
        return make_unexpected(RegisterError::NameAlreadyRegistered);

    auto makerLambda = []() {
        std::unique_ptr<nf::DataNode> uptr = std::make_unique<DataNodeImpl<T>>();
        return uptr;
    };

    m_dataNodeRegistry[namePath] = std::move(makerLambda);

    return {};
}

template <class Node>
Expected<void, RegisterError> FlowModule::registerCustomNode(const std::string& namePath)
{
    static_assert(std::is_base_of_v<nf::FlowNode, Node>,
                  "<Node> needs to be of base <nf::FlowNode>");

    auto parseResult = helperParseNamePath(namePath);
    if (!parseResult)
        return make_unexpected(parseResult.error());

    auto& [tmp, categoryName] = parseResult.value();

    auto makerLambda = []() {
        std::unique_ptr<nf::FlowNode> uptr = std::make_unique<Node>();
        return uptr;
    };

    m_flowNodeRegistry[namePath] = std::move(makerLambda);
    return {};
}

template <auto func>
nf::Expected<void, nf::RegisterError> FlowModule::registerFunction(const std::string& namePath)
{
    static_assert(!std::is_member_function_pointer<decltype(func)>::value,
                  "Callable must be lambda or free function");

    if (namePath.empty())
        return make_unexpected(RegisterError::EmptyNamePath);

    if (m_flowNodeRegistry.contains(namePath) || m_dataNodeRegistry.contains(namePath))
        return make_unexpected(RegisterError::NameAlreadyRegistered);

    FunctorNode<func>::staticNodeName = namePath;

    auto makerLambda = []() {
        std::unique_ptr<nf::FlowNode> uptr = std::make_unique<FunctorNode<func>>();
        return uptr;
    };

    m_flowNodeRegistry[namePath] = std::move(makerLambda);

    return {};
}

template <auto Callable>
Expected<void, RegisterError> FlowModule::registerConversion(
    const std::string& namePath, const InOutPortName& portName /* = {}*/)
{
    static_assert(!std::is_member_function_pointer<decltype(Callable)>::value,
                  "Callable must be lambda or free function");
    auto parseResult = helperParseNamePath(namePath);
    if (!parseResult)
        return make_unexpected(parseResult.error());

    auto& [baseName, categoryName] = parseResult.value();

    using FSig_t = FuncSignature<decltype(std::function{ Callable })>;
    using To_t = FSig_t::ReturnType_t;
    using From_ts = FSig_t::ParamTypes_t;
    static_assert(std::tuple_size_v<From_ts> == 1,
                  "Callable must be of signature: 'ToType Callable(FromType)'");
    using From_t = std::tuple_element_t<0, From_ts>; // top-most const already removed;

    ConversionNodeImpl<From_t, To_t, Callable>::staticNodeName = baseName;
    ConversionNodeImpl<From_t, To_t, Callable>::staticInputPortName = portName.inputName;
    ConversionNodeImpl<From_t, To_t, Callable>::staticOutputPortName = portName.outputName;

    auto makerLambda = []() {
        std::unique_ptr<nf::FlowNode> uptr =
            std::make_unique<ConversionNodeImpl<From_t, To_t, Callable>>();
        return uptr;
    };

    m_flowNodeRegistry[namePath] = std::move(makerLambda);

    return {};
}
} // namespace nf