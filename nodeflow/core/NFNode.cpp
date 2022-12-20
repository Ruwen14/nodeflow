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

	MakeLinkResult NFNode::makeLink(PortIndex fromOutIndex, NFNode* toNode, PortIndex toInIndex)
	{
// 		if (fromOutIndex == -1 || toInIndex == -1)
// 			return {MakeLinkResult::PortIndexInvalid};
// 
// 		if (fromOutIndex < m_outputPorts.size() || toInIndex < toNode->m_inputPorts.size())
// 			return { MakeLinkResult::PortIndexInvalid };
// 
// 		if (toNode == nullptr)
// 			return { MakeLinkResult::NodeInvalid };
// 
// 		if (toNode->m_inputPorts[toInIndex].hasValidLink())
// 			return { MakeLinkResult::PortAlreadyLinked };
// 
// 		for (const auto& link : m_outputPorts[fromOutIndex].links())
// 		{
// 			if (link.valid())
// 			{
// 				if (link.targetNode == toNode && link.targetIndex == toInIndex)
// 				{
// 					return { MakeLinkResult::PortAlreadyLinked; }
// 				}
// 			}
// 		}

	}


}
