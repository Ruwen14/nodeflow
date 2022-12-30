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
#include <string_view>
#include <sstream>

#include "typedefs.hpp"
#include "core/Object.hpp"
#include "core/NodePort.hpp"
#include "core/Error.hpp"
#include "utility/Expected.hpp"
#include "utility/TypenameAtlas.hpp"


namespace nf
{
	enum class ConnectionError
	{
		PortAlreadyLinked,
		PortIndexInvalid,
		NodeInvalid,
		UnequalPortTypes
	};

	enum class ConnectionPolicy
	{
		InputToOutput,
		OutputToInput
	};




#define NF_NODE_NAME(name)				\
public:									\
std::string nodeName() const override	\
{										\
	return std::string(name);			\
}										\




	class Node : public Object
	{
	public:
		Node() = default;
		virtual ~Node() = default;

	public: // Or better private and friend FlowScript
		virtual std::string nodeName() const { return "NFNode"; }

		virtual bool serialize(std::ostringstream& archive, 
			PortDirection dir, PortIndex index) const;

// 	protected:
	public:
		virtual Expected<void, Error> setup() { return {}; }

		virtual void process() {}

	public:
		size_t portCount(PortDirection dir) const noexcept;

		const std::vector<InputPortHandle>& getInputPortList() const noexcept;

		const std::vector<OutputPortHandle>& getOutputPortList() const noexcept;

		const InputPortHandle& getInputPort(PortIndex index) const;

		const OutputPortHandle& getOutputPort(PortIndex index) const;

		const InputPortHandle* findInputPort(std::string_view name) const;

		const InputPortHandle* findInputPort(PortIndex index) const;

		const OutputPortHandle* findOutputPort(std::string_view name) const;

		const OutputPortHandle* findOutputPort(PortIndex index) const;

		void formatLinkageTree(std::ostringstream& stream) const;


		bool inputPortLinked(PortIndex index) const;

		bool outputPortLinked(PortIndex index) const;


		/**
		 * @brief Inter-Connects two ports. 
		 * @param policy 
		 * @param originPort 
		 * @param targetNode 
		 * @param targetPort 
		 * @return 
		*/
		Expected<void, ConnectionError> makeConnection(ConnectionPolicy policy, 
													   PortIndex originPort, 
													   Node& targetNode, 
													   PortIndex targetPort);

		/**
		 * @brief Removes a specific Connection
		 * @param policy 
		 * @param originPort 
		 * @param targetNode 
		 * @param targetPort 
		 * @return 
		*/
		bool breakConnection(ConnectionPolicy policy, 
							  PortIndex originPort, 
							  Node& targetNode, 
							  PortIndex targetPort);

		void breakAllConnections(PortDirection dir = PortDirection::Both);


// 		bool breakAllConnections

		/**
		 * @brief Removes all links associated with this node(targetNode)
		 * @param targetNote 
		*/
		void clearAssociatedLinks(Node* targetNode);

	protected:
		template<typename T>
		bool addPort(InputPort<T>& p, const std::string& caption = "");

		template<typename T>
		bool addPort(OutputPort<T>& p, const std::string& caption = "");

		template<typename T>
		const T* getInputData(const InputPort<T>& p) const
		{
			const PortLink& link = m_inputPorts[p.m_portIndex].m_link;
			// No input connection
			if (!link.valid())
				return nullptr;

			const detail::DataHandle& datahandle = link.targetNode->m_outputPorts[link.targetIndex].m_dataHandle;
			return datahandle.get<T>();
		}

		template<typename T, typename Ty>
		void setOutputData(OutputPort<T>& p, Ty&& val) noexcept
		{
// 			pprint(nf::type_name<decltype(val)>());
			p.setValue(std::forward<Ty>(val));
		}

	private:

		/**
		 * @brief API to make connection from the perspective of the output of a node
		 * @param outPort 
		 * @param toNode 
		 * @param toInPort 
		 * @return 
		*/
		[[nodiscard]] Expected<void, ConnectionError> makeInterlinkedConnection(PortIndex outPort, 
																				Node& toNode, 
																				PortIndex toInPort);


		/**
		 * @brief API to break connection from the perspective of the output of a node
		 * @param outPortIndex 
		 * @param toNode 
		 * @param toInPortIndex 
		 * @return 
		*/
		[[nodiscard]] bool breakInterlinkedConnection(PortIndex outPortIndex, Node& toNode, PortIndex toInPortIndex);

		void clearInterlinkedPorts(PortIndex outPort);




	private:
		std::vector<OutputPortHandle> m_outputPorts;
		std::vector<InputPortHandle> m_inputPorts;

	};

	template<typename T>
	bool Node::addPort(InputPort<T>& p, const std::string& caption /*= ""*/)
	{
		if (p.assigned())
			return false;
		m_inputPorts.emplace_back(p.typeID, caption);
		p.setIndex(static_cast<int>(m_inputPorts.size() - 1));

		auto& atlas = TypenameAtlas::instance();
		atlas.add<T>();


		return true;
	}


	template<typename T>
	bool Node::addPort(OutputPort<T>& p, const std::string& caption /*= ""*/)
	{
		if (p.assigned())
			return false;
		m_outputPorts.emplace_back(p.value, p.typeID, caption);
		p.setIndex(static_cast<int>(m_outputPorts.size() - 1));

		auto& atlas = TypenameAtlas::instance();
		atlas.add<T>();

		return true;
	}

}