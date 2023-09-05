
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
#include <string>
#include <map>
#include <set>
#include <memory>
#include <type_traits>
#include <functional>
#include <utility>

#include "typedefs.hpp"
#include "core/type_tricks.hpp"
#include "utility/Expected.hpp"
#include "core/Error.hpp"
#include "core/Node.hpp"
#include "nodes/DataNode.hpp"
#include "nodes/FlowNode.hpp"
#include "nodes/FunctorNode.hpp"
#include "nodes/EventNode.hpp"
#include "nodes/ConversionNode.hpp"

#include "../3rdparty/cpputils/prettyprint.h"
using namespace cpputils;




namespace nf {

	class Node;
	class FlowNode;

	using VariableNode = DataNode;

	template<typename T>
	using VariableNodeImpl = DataNodeImpl<T>;

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

		using FlowNodeCreatorMap = std::map<std::string, FlowNodeCreator>;
		using DataNodeCreatorMap = std::map<std::string, DataNodeCreator>;

	public:
		FlowModule() = default;
		explicit FlowModule(const std::string& moduleName)
			: m_moduleName(moduleName) {}

	public:
		template<typename T>
		Expected<void, RegisterError> registerType(const std::string& namePath);

		template<class Node> 
		Expected<void, RegisterError> registerCustomNode(const std::string& namePath);

		template<auto func> 
		Expected<void, RegisterError> registerFunction(const std::string& namePath, const FunctorPortNames& portNames = {});

		template<class ClassNode>
		Expected<void, Error> registerClass(const std::string& category);

		template<auto Callable> 
		Expected<void, RegisterError> registerConversion(const std::string& namePath, const InOutPortName& portName = {});

		template<class Node> 
		Expected<void, Error> registerStartEventNode(const std::string& namePath);


		void setModuleName(const std::string& name);

		std::string moduleName() const;

		const std::set<std::string>& categories() const;

		std::set<std::string> registered() const;

		const FlowNodeCreatorMap& nodeCreators() const;

		const DataNodeCreatorMap& dataCreators() const;

	private:
		
		Expected<std::pair<std::string, std::string>, RegisterError> helperParseNamePath(const std::string& namePath) const;

	public:
		std::string m_moduleName;
		FlowNodeCreatorMap m_flowNodeCreators; // might use map for reduced memory consumption
		DataNodeCreatorMap m_dataNodeCreators;
		std::set<std::string> m_categoryNames;
		

	};


	template<typename T>
	Expected<void, RegisterError> FlowModule::registerType(const std::string& namePath)
	{
		static_assert(!std::is_base_of<nf::Node, T>::value, "<T> is not allowed to be of base <nf::Node>");

		auto parseResult = helperParseNamePath(namePath);
		if (!parseResult)
			return make_unexpected(parseResult.error());

		auto& [baseName, categoryName] = parseResult.value();

		if (!categoryName.empty()) m_categoryNames.insert(categoryName);

		DataNodeImpl<T>::staticNodeName = baseName;
		auto makerLambda = []() {
			std::unique_ptr<nf::DataNode> uptr = std::make_unique<DataNodeImpl<T>>();
			uptr->assignTypeID(type_id<DataNodeImpl<T>>());
			return uptr;
		};

		m_dataNodeCreators[namePath] = std::move(makerLambda);
		return {};
	}
	
	template<class Node> 
	Expected<void, RegisterError> FlowModule::registerCustomNode(const std::string& namePath)
	{
		static_assert(std::is_base_of_v<nf::FlowNode, Node>, "<Node> needs to be of base <nf::FlowNode>");

		auto parseResult = helperParseNamePath(namePath);
		if (!parseResult)
			return make_unexpected(parseResult.error());

		auto& [tmp, categoryName] = parseResult.value();

		if (!categoryName.empty()) m_categoryNames.insert(categoryName);

		auto makerLambda = []() { 
			std::unique_ptr<nf::FlowNode> uptr = std::make_unique<Node>();
			uptr->assignTypeID(type_id<Node>());
			return uptr;
		};

		m_flowNodeCreators[namePath] = std::move(makerLambda);
		return {};
	}

	template<auto func> 
	Expected<void, RegisterError> FlowModule::registerFunction(const std::string& namePath, const FunctorPortNames& portNames/* = {}*/)
	{
		static_assert(!std::is_member_function_pointer<decltype(func)>::value, "Callable must be lambda or free function");

		auto parseResult = helperParseNamePath(namePath);
		if (!parseResult)
			return make_unexpected(parseResult.error());

		auto& [baseName, categoryName] = parseResult.value();

		if (!categoryName.empty()) m_categoryNames.insert(categoryName);

		FunctorNode<func>::staticNodeName = baseName;
		FunctorNode<func>::staticResultPortName = portNames.outputName;
		if (!portNames.inputNames.empty()) FunctorNode<func>::staticArgPortNames = portNames.inputNames;

		auto makerLambda = []() {
			std::unique_ptr<nf::FlowNode> uptr = std::make_unique<FunctorNode<func>>();
			uptr->assignTypeID(type_id<FunctorNode<func>>());
			return uptr;
		};

		m_flowNodeCreators[namePath] = std::move(makerLambda);

		return {};
	}
	template<class ClassNode>
	Expected<void, Error> FlowModule::registerClass(const std::string& category)
	{
		return {};
	}

	template<auto Callable> 
	Expected<void, RegisterError> FlowModule::registerConversion(const std::string& namePath, const InOutPortName& portName/* = {}*/)
	{
		static_assert(!std::is_member_function_pointer<decltype(Callable)>::value, "Callable must be lambda or free function");
		auto parseResult = helperParseNamePath(namePath);
		if (!parseResult)
			return make_unexpected(parseResult.error());

		auto& [baseName, categoryName] = parseResult.value();

		if (!categoryName.empty()) m_categoryNames.insert(categoryName);

		using FSig_t = FuncSignature<decltype(std::function{ Callable }) > ;
		using To_t = FSig_t::ReturnType_t;
		using From_ts = FSig_t::ParamTypes_t;
		static_assert(std::tuple_size_v<From_ts> == 1, "Callable must be of signature: 'ToType Callable(FromType)'");
		using From_t = std::tuple_element_t<0, From_ts>; // top-most const already removed;

		ConversionNodeImpl<From_t, To_t, Callable>::staticNodeName = baseName;
		ConversionNodeImpl<From_t, To_t, Callable>::staticInputPortName = portName.inputName;
		ConversionNodeImpl<From_t, To_t, Callable>::staticOutputPortName = portName.outputName;

		auto makerLambda = []() {
			std::unique_ptr<nf::FlowNode> uptr = std::make_unique<ConversionNodeImpl<From_t, To_t, Callable>>();
			uptr->assignTypeID(type_id<ConversionNodeImpl<From_t, To_t, Callable>>());
			return uptr;
		};

		m_flowNodeCreators[namePath] = std::move(makerLambda);

		return {};
	}

	template<class Node> 
	Expected<void, Error> FlowModule::registerStartEventNode(const std::string& namePath)
	{
		using Event_t = Node::Event_t;
		static_assert(std::is_base_of_v<nf::EventNode<Event_t>, Node>, "<Node> needs to be of base <nf::EventNode<T>>");

		return {};
	}

}