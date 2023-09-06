#include "nodeflow/nodes/FlowNode.hpp"

namespace nf
{
	NodeArchetype FlowNode::getArchetype() const
	{
		return NodeArchetype::FlowNode;
	}

	std::string FlowNode::nodeName() const
	{
		return "FlowNode";
	}

	bool FlowNode::onEvent(FlowEvent* event)
	{
		NF_UNUSED(event);
		std::cout << ("FlowEvent") << " \n";
		return false;
	}

	void FlowNode::breakFlow(FlowDirection dir)
	{
		if (dir == FlowDirection::Next)
			m_outExecPort.execLink.breakLink();
		else
			m_inExecPort.execLink.breakLink();
	}

	FlowPort& FlowNode::defaultFlowPort(FlowDirection dir)
	{
		if (dir == FlowDirection::Before)
			return m_inExecPort;

		return m_outExecPort;
	}

	bool FlowNode::hasAdditionalFlowPorts() const
	{
		return additionalFlowPorts().size() != 0;
	}

	std::vector<FlowPort*> FlowNode::additionalFlowPorts() const
	{
		return {};
	}

	std::string FlowNode::flowPortName(FlowDirection dir, PortIndex index) const
	{
		NF_UNUSED(index);
		NF_UNUSED(dir);
		return {};
	}
}