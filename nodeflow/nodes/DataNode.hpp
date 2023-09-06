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
#pragma region DataNode

	class DataNode : public Node
	{
	public:
		DataNode() = default;

		std::string nodeName() const override;

		NodeArchetype getArchetype() const final;

		virtual std::unique_ptr<FlowNode> constructSetterNode() const = 0;

		virtual typeid_t underlyingDataTypeID() const = 0;

		// 		virtual bool trySetDataFrom(std::any data);

				// virtual std::unique_ptr<FlowNode> constructGetter() const = 0;

		template<typename T>
		static bool setData(DataNode& node, T&& data);

		template<typename T>
		static const std::optional<T> getData(DataNode& node);
	};

#pragma endregion DataNode

#pragma region DataSetterNode

	template<typename Type>
	class DataSetterNode : public FlowNode
	{
	public:
		DataSetterNode();

		std::string nodeName() const override;

		Expected<void, Error> setup() override;

		void process() override
		{
			auto dataNodeData = getInputDataMutable(m_dataNodeData);
			auto setValue = getInputData(m_setValue);

			if (dataNodeData && setValue)
				*dataNodeData = *setValue;
		}

		NodeArchetype getArchetype() const override;

	private:
		InputPort<Type> m_dataNodeData;
		InputPort<Type> m_setValue;
	};

	template<typename Type>
	std::string nf::DataSetterNode<Type>::nodeName() const
	{
		return "DataSetterNode";
	}

	template<typename Type>
	Expected<void, Error> DataSetterNode<Type>::setup()
	{
		addPort(m_dataNodeData);
		addPort(m_setValue);

		return {};
	}

	template<typename Type>
	DataSetterNode<Type>::DataSetterNode()
	{
		// Reserve exact amount of memory we need in port vector
		allocateExpectedPortCount(PortDirection::Input, 2);
		allocateExpectedPortCount(PortDirection::Output, 0);
	}

	template<typename Type>
	NodeArchetype DataSetterNode<Type>::getArchetype() const
	{
		return NodeArchetype::FlowNode;
	}

#pragma endregion DataSetterNode

#pragma region DataNodeImpl

	template<typename Type>
	class DataNodeImpl : public DataNode
	{
	public:
		static std::string staticNodeName;

	public:
		DataNodeImpl();
		DataNodeImpl(Type&& value);

		Expected<void, Error> setup() override;

		std::string nodeName() const override;

		bool streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive) final;

		std::unique_ptr<FlowNode> constructSetterNode() const final;

		typeid_t underlyingDataTypeID() const final;

		inline const Type& data() const noexcept { return m_data.value; }

		inline void setData(Type&& data) noexcept { m_data.value = std::forward<Type>(data); }

	private:
		OutputPort<Type> m_data;
	};

	template <typename Type>
	std::string DataNodeImpl<Type>::staticNodeName = "DataNode";

	template<typename Type>
	DataNodeImpl<Type>::DataNodeImpl(Type&& value)
	{
		m_data.setValue(std::forward<Type>(value));
		// Reserve exact amount of memory we need in port vector
		allocateExpectedPortCount(PortDirection::Input, 0);
		allocateExpectedPortCount(PortDirection::Output, 1);
	}

	template<typename Type>
	DataNodeImpl<Type>::DataNodeImpl()
	{
		// Reserve exact amount of memory we need in port vector
		allocateExpectedPortCount(PortDirection::Input, 0);
		allocateExpectedPortCount(PortDirection::Output, 1);
	}

	template<typename Type>
	Expected<void, Error> DataNodeImpl<Type>::setup()
	{
		addPort(m_data);
		return {};
	}

	template<typename Type>
	std::string DataNodeImpl<Type>::nodeName() const
	{
		return staticNodeName;
	}

	template<typename Type>
	bool DataNodeImpl<Type>::streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive)
	{
		if (index != 0)
			return false;
		if (flag == StreamFlag::WriteTo)
			return m_data.serialize(archive);
		return m_data.deserialize(archive);
	}

	template<typename Type>
	std::unique_ptr<FlowNode> DataNodeImpl<Type>::constructSetterNode() const
	{
		return std::make_unique<DataSetterNode<Type>>();
	}

	template<typename Type>
	typeid_t DataNodeImpl<Type>::underlyingDataTypeID() const
	{
		return m_data.typeID;
	}

#pragma endregion DataNodeImpl

	template<typename T>
	bool DataNode::setData(DataNode& node, T&& data)
	{
		static constexpr typeid_t setDataTypeID = type_id<T>();
		if (node.underlyingDataTypeID() != setDataTypeID)
			return false;

		NF_ASSERT(dynamic_cast<DataNodeImpl<T>*>(&node) != nullptr, "Oops, this shoudln't happen. type_id<T>() might have a hash collision?");
		auto& actualNode = static_cast<DataNodeImpl<T>&>(node);
		actualNode.setData(std::forward<T>(data));
		return true;
	}

	template<typename T>
	const std::optional<T> DataNode::getData(DataNode& node)
	{
		static constexpr typeid_t getDataTypeID = type_id<T>();
		if (node.underlyingDataTypeID() != getDataTypeID)
			return std::nullopt;

		NF_ASSERT(dynamic_cast<DataNodeImpl<T>*>(&node) != nullptr, "Oops, this shoudln't happen. type_id<T>() might have a hash collision?");
		auto& actualNode = static_cast<DataNodeImpl<T>&>(node);
		return actualNode.data();
	}
}