
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
#include <memory>

#include "typedefs.hpp"
#include "core/Error.hpp"
#include "utility/Expected.hpp"
#include "script/FlowModule.hpp"
#include "nodes/EventNode.hpp"

namespace nf
{	
	enum class CreateNodeError
	{
		UnknownNodeName,
		NodeNameFalseCategory
	};

	using NodeHandle = UUID;


	class FlowScript
	{

	public:
		FlowScript(std::shared_ptr<FlowModule> scriptModule)
			: m_scriptModule(std::move(scriptModule))
		{}

// 		Node* variables() const;
		
		void precomputeExecutionOrder();

		/**
		 * @brief Broadcasts custom event to all nodes within script.
		 Nodes can react on event by overriding onEvent method.
		 * @tparam EventType 
		 * @tparam ...EventArgs 
		 * @param ...eventArgs 
		*/
		template<typename EventType, typename... EventArgs>
		void broadcastEvent(EventArgs&&... eventArgs)
		{
			EventType event(std::forward<EventArgs>(eventArgs)...);
			for (auto& node : m_callablesNodes)
				node->onEvent(&event);
		}


		nf::Node* findNode(NodeHandle node) const;

		bool hasNode(NodeHandle uuid) const;

		template<typename NodeType, class Callable>
		void forEach(Callable callable)
		{
			static constexpr typeid_t nodeTypeID = type_id<NodeType>();

			std::for_each(m_callablesNodes.begin(), m_callablesNodes.end(), [](std::unique_ptr<FlowNode>& node) 
			{
			});
		}


		Expected<NodeHandle, Error> spawnNode(const std::string& namePath);

		bool removeNode(NodeHandle node);

		Expected<void, ConnectionError> connectPorts(NodeHandle outNodeUUID, PortIndex outPort, 
													 NodeHandle inNodeUUID, PortIndex inPort, 
													 ConversionPolicy conv = ConversionPolicy::DontAddConversion);


		bool disconnectPorts(NodeHandle outNodeUUID, PortIndex outPort, NodeHandle inNodeUUID, PortIndex inPort);

		bool connectFlow(NodeHandle outNodeUUID, NodeHandle inNodeUUID);

		bool disconnectFlow(NodeHandle outNode, NodeHandle inNode);



		void setStartEvent();

		void setStartEventNode();


		std::string nodeOutputAsStr(NodeHandle node, PortIndex index);

		void setNodeOutputFromStr(NodeHandle node, PortIndex index, const std::string& str);


		StartEventNode& startEventNode() const;
		
		bool build();

		void run();
		
		/*
		ExpectedRef<FlowNode, Error> spawnNode(const std::string& namePath);

		ExpectedRef<DataNode, Error> spawnType(const std::string& namePath);
		*/

		/*
		Expected<void, ConnectionError> connectNodes(Node& outNode, PortIndex outPort, 
												Node& inNode, PortIndex inPort);

		bool disconnectNodes(Node& outNode, PortIndex outPort, Node& inNode, PortIndex inPort);
		*/
	private:

		nf::Node* findNode(NodeHandle uuid, std::pair<int, size_t>& pos) const;

		nf::Node* findPortConversionNode(typeid_t fromType, typeid_t toType) const;

		bool isUUIDUnique(UUID uuid) const;


		Expected<NodeHandle, Error> createNode(const std::string& namePath);

		Expected<NodeHandle, Error> createVariable(const std::string& namePath);

		bool debugAllConnectionsRemovedTo(nf::Node* node) const;


	public :
		std::vector<std::unique_ptr<FlowNode>> m_callablesNodes;
		std::vector<std::unique_ptr<DataNode>> m_variableNodes;
		std::shared_ptr<FlowModule> m_scriptModule;

// 		std::vector<BuildError> errorList
	};
}
