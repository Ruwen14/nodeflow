#include "core/NFNode.hpp"


namespace nf
{
	bool NFNode::serialize(std::ostringstream& serializer, PortDirection dir, PortIndex index) const
	{
		return false;
	}

	size_t NFNode::portCount(PortDirection dir) const noexcept
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

	const std::vector<detail::InputPortContext>& NFNode::getInputPortList() const noexcept
{
		return m_inputPorts;
	}

	const std::vector<detail::OutputPortContext>& NFNode::getOutputPortList() const noexcept
{
		return m_outputPorts;
	}


	const detail::InputPortContext& NFNode::getInputPort(PortIndex index) const
	{
		NF_ASSERT(index != -1, "Invalid port index");
		NF_ASSERT(index < m_inputPorts.size(), "Port index out of range");
		return m_inputPorts[index];
	}

	const detail::OutputPortContext& NFNode::getOutputPort(PortIndex index) const
	{
		NF_ASSERT(index != -1, "Invalid port index");
		NF_ASSERT(index < m_outputPorts.size(), "Port index out of range");
		return m_outputPorts[index];
	}

	const detail::InputPortContext* NFNode::findInputPort(std::string_view name) const
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

	const detail::OutputPortContext* NFNode::findOutputPort(std::string_view name) const
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

	void NFNode::formatLinkageTree(std::ostringstream& stream) const
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

	LinkageResult NFNode::makeLink(PortIndex fromOutIndex, NFNode* toNode, PortIndex toInIndex)
	{
		// Check if Link between Ports can be made ------------------
		if (toNode == nullptr)
			return { LinkageResult::ERROR_NodeInvalid };

		if (fromOutIndex == -1 || toInIndex == -1 
			|| !(fromOutIndex < m_outputPorts.size()) || !(toInIndex < toNode->m_inputPorts.size()))
			return {LinkageResult::ERROR_PortIndexInvalid};

		if (toNode->m_inputPorts[toInIndex].hasValidLink())
			return { LinkageResult::ERROR_PortAlreadyLinked };

		auto fromPortType = m_outputPorts[fromOutIndex].typeID();
		auto toPortType = toNode->m_inputPorts[toInIndex].typeID();

		if (m_outputPorts[fromOutIndex].typeID() != toNode->m_inputPorts[toInIndex].typeID())
			return { LinkageResult::ERROR_UnequalPortTypes };
		// ----------------------------------------------------------
		
		auto futurePortLink = nf::detail::PortLink(toInIndex, toNode);
		auto& port = m_outputPorts[fromOutIndex];
		const auto& portLinks = port.links();

		if (std::find(portLinks.begin(), portLinks.end(), futurePortLink) != portLinks.end())
			return { LinkageResult::ERROR_PortAlreadyLinked };

		port.createLink(toInIndex, toNode);

		// Add this Link to the output of the target node
		auto& targetPort = toNode->m_inputPorts[toInIndex];
		targetPort.makeLink(fromOutIndex, this);

		return { LinkageResult::Success };
	}



}
