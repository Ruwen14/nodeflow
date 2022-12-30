#include "core/Node.hpp"
#include "../3rdparty/cpputils/prettyprint.h"
using namespace cpputils;

namespace nf
{
	bool Node::serialize(std::ostringstream& archive, PortDirection dir, PortIndex index) const
	{
		return false;
	}

	size_t Node::portCount(PortDirection dir) const noexcept
	{
		switch (dir)
		{
		case PortDirection::Input:
			return m_inputPorts.size();
		case PortDirection::Output:
			return m_outputPorts.size();
		default:
			return 0;
		}
	}

	const std::vector<InputPortHandle>& Node::getInputPortList() const noexcept
	{
		return m_inputPorts;
	}

	const std::vector<OutputPortHandle>& Node::getOutputPortList() const noexcept
	{
		return m_outputPorts;
	}


	const InputPortHandle& Node::getInputPort(PortIndex index) const
	{
		NF_ASSERT(index != -1, "Invalid port index");
		NF_ASSERT(index < m_inputPorts.size(), "Port index out of range");
		return m_inputPorts[index];
	}

	const OutputPortHandle& Node::getOutputPort(PortIndex index) const
	{
		NF_ASSERT(index != -1, "Invalid port index");
		NF_ASSERT(index < m_outputPorts.size(), "Port index out of range");
		return m_outputPorts[index];
	}

	const InputPortHandle* Node::findInputPort(std::string_view name) const
	{
		for (const auto& port : m_inputPorts)
		{
			if (name == port.name())
			{
				return &port;
			}
		}
		return nullptr;
	}

	const InputPortHandle* Node::findInputPort(PortIndex index) const
	{
		if (index == -1 || !(index < m_inputPorts.size()))
			return nullptr;

		const auto& port = m_inputPorts[index];
		return &port;
	}

	const OutputPortHandle* Node::findOutputPort(std::string_view name) const
	{
		for (const auto& port : m_outputPorts)
		{
			if (name == port.name())
			{
				return &port;
			}
		}
		return nullptr;
	}

	const OutputPortHandle* Node::findOutputPort(PortIndex index) const
	{
		if (index == -1 || !(index < m_outputPorts.size()))
			return nullptr;

		const auto& port = m_outputPorts[index];
		return &port;
	}

	void Node::formatLinkageTree(std::ostringstream& stream) const
	{
	    stream << "LinkageTree for [Node:" << nodeName() << " @" << this <<"]\n";
		stream << "{\n";
		stream << "    [Output Ports]:\n";
		for (const auto& oPort : m_outputPorts)
		{
			stream << "\t>[Port:" << oPort.name() << "] Link Count: " << oPort.linkCount() <<"\n";
			for (const auto& link : oPort.links())
			{
				stream << "\t\t\t---<Link>--> ";

				if (!link.valid())
					stream << " Invalid";
				stream << "To [Port:" << link.targetNode->getInputPortList()[link.targetIndex].name() 
					<< "][" << link.targetIndex <<
					"] of [Node:" << link.targetNode->nodeName() << " @" << link.targetNode << "]\n";
					
			}

		}
		stream << "\n    [Input Ports]:\n";
		for (const auto& iPort : m_inputPorts)
		{
			stream << "\t>[Port:" << iPort.name() << "] ---<Link>--> ";
			auto link = iPort.link();
			if (!link.valid())
				stream << "NOT ASSIGNED\n";
			else {
				stream << "To [Port:" << link.targetNode->getOutputPortList()[link.targetIndex].name() 
					<< "][" << link.targetIndex << "] of [Node:" <<
					link.targetNode->nodeName() << " @" << link.targetNode << "]\n";
			}

		}
		stream << "}\n";
	}

	Expected<void, ConnectionError> Node::makeInterlinkedConnection(PortIndex outPort, Node& toNode, PortIndex toInPort)
	{
		// Check if Link between Ports can be made ------------------
// 		if (toNode == nullptr)
// 			return { ConnectionError::ERROR_NodeInvalid };


// 		std::cout << (&toNode) << "\n";
		if (outPort == -1 || toInPort == -1 
			|| !(outPort < m_outputPorts.size()) || !(toInPort < toNode.m_inputPorts.size()))
			return make_unexpected(ConnectionError::PortIndexInvalid);

		if (toNode.m_inputPorts[toInPort].hasValidLink())
			return make_unexpected( ConnectionError::PortAlreadyLinked );

		auto fromPortType = m_outputPorts[outPort].typeID();
		auto toPortType = toNode.m_inputPorts[toInPort].typeID();
		if (fromPortType != toPortType)
			return make_unexpected( ConnectionError::UnequalPortTypes );
		// ----------------------------------------------------------
		
		auto& originPort = m_outputPorts[outPort];
		auto& targetPort = toNode.m_inputPorts[toInPort];

		if (std::find(originPort.links().begin(), originPort.links().end(), PortLink(toInPort, &toNode)) != originPort.links().end())
			return make_unexpected( ConnectionError::PortAlreadyLinked );


		originPort.createLink(toInPort, &toNode);
		// Add this Link to the output of the target node
		targetPort.makeLink(outPort, this);

		return {};
	}



	bool Node::breakInterlinkedConnection(PortIndex outPortIndex, Node& toNode, PortIndex toInPortIndex)
	{
		NF_ASSERT((outPortIndex != -1 && toInPortIndex != -1), "Invalid port index");
		NF_ASSERT(outPortIndex < m_outputPorts.size(), "Port index out of range");


		OutputPortHandle& outPort = m_outputPorts[outPortIndex];
		PortLink linkToBeRemoved(toInPortIndex, &toNode);
		NF_ASSERT(outPort.hasLink(linkToBeRemoved), "Link doesn't exist");
		if (!outPort.hasLink(linkToBeRemoved))
			return false;

		// Notify input port of linked node
		NF_ASSERT(toInPortIndex < toNode.m_inputPorts.size(), "Port index out of range");
		InputPortHandle& targetInPort = toNode.m_inputPorts[toInPortIndex];
		targetInPort.breakLink();
		outPort.removeLink(linkToBeRemoved);
		return true;
	}

	bool Node::inputPortLinked(PortIndex index) const
	{
		if (index == -1 || !(index < m_inputPorts.size()) )
			return false;

		const auto& port = m_inputPorts[index];
		return port.hasValidLink();
	}

	bool Node::outputPortLinked(PortIndex index) const
	{
		if (index == -1 || !(index < m_outputPorts.size()) )
			return false;

		const auto& port = m_outputPorts[index];

		if (port.linkCount() == 0)
			return false;

		for (const auto& link : port.links())
		{
			if (!link.valid())
				return false;
		}
		return true;
	}

	Expected<void, ConnectionError> Node::makeConnection(ConnectionPolicy policy, 
														   PortIndex originPort,
														   Node& targetNode,
														   PortIndex targetPort)
	{
		if (policy == ConnectionPolicy::OutputToInput)
			return makeInterlinkedConnection(originPort, targetNode, targetPort);
		return targetNode.makeInterlinkedConnection(targetPort, *this, originPort);
	}
	

	void Node::clearInterlinkedPorts(PortIndex outPort)
	{
		NF_ASSERT(outPort != -1, "Invalid port index");
		NF_ASSERT(outPort < m_outputPorts.size(), "Port index out of range");

		OutputPortHandle& port = m_outputPorts[outPort];

		// Clear all links from other nodes to this port
		for (const PortLink& link : port.links())
		{
			if (!link.valid())
				continue;

			InputPortHandle& linkedTargetPort = link.targetNode->m_inputPorts[link.targetIndex];
			linkedTargetPort.breakLink();
		}

		// Clear all links to other nodes
		port.breakAllLinks();

	}


	bool Node::breakConnection(ConnectionPolicy policy, 
								  PortIndex originPort,
								  Node& targetNode,
								  PortIndex targetPort)
	{
		if (policy == ConnectionPolicy::OutputToInput)
			return breakInterlinkedConnection(originPort, targetNode, targetPort);
		return targetNode.breakInterlinkedConnection(targetPort, *this, originPort);
	}

	void Node::breakAllConnections(PortDirection dir /*= PortDirection::Both*/)
	{
		if (dir	== PortDirection::Input || dir == PortDirection::Both)
		{
			for (size_t i = 0; i < m_inputPorts.size(); i++)
			{
				InputPortHandle& inPort = m_inputPorts[i];
				PortLink inLink = inPort.link();
				if (!inLink.valid())
					continue;

				this->breakConnection(ConnectionPolicy::InputToOutput,
									  static_cast<PortIndex>(i),
									  *inLink.targetNode,
									  inLink.targetIndex);
			}
		}

		if (dir == PortDirection::Output || dir == PortDirection::Both)
		{
			for (size_t i = 0; i < m_outputPorts.size(); i++)
			{
				OutputPortHandle& outPort = m_outputPorts[i];
				for (const PortLink& outLink : outPort.links())
				{
					NF_ASSERT(outLink.valid(), "Out-Link not valid");
					this->breakConnection(ConnectionPolicy::OutputToInput, 
										  static_cast<PortIndex>(i), 
										  *outLink.targetNode, 
										  outLink.targetIndex);

				}
			}
		}
	}

}
