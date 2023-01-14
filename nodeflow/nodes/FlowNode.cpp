#include "nodes/FlowNode.hpp"

namespace nf
{

	NodeArchetype FlowNode::getArchetype() const
	{
		return NodeArchetype::FlowNode;
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
			m_linkNext.breakLink();
		else
			m_linkBefore.breakLink();

	}

}
