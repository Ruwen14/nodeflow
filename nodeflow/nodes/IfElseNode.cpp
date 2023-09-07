#include "nodeflow/nodes/IfElseNode.hpp"

namespace nf
{
	IfElseNode::IfElseNode()
	{
		allocateExpectedPortCount(PortDirection::Input, 1);
		allocateExpectedPortCount(PortDirection::Output, 0);
	}

	std::string IfElseNode::nodeName() const
	{
		return "IfElse";
	}

	NodeArchetype IfElseNode::getArchetype() const
	{
		return NodeArchetype::Lang_IfElse;
	}

	nf::ErrorOr<void> IfElseNode::setup()
	{
		addPort(m_condition);

		return {};
	}

	void IfElseNode::process()
	{
		auto cond = getInputData(m_condition);

		if (cond == nullptr)
			return;
	}

	void IfElseNode::setExecFlowIf(FlowNode& node)
	{
	}

	void IfElseNode::setExecFlowElse(FlowNode& node)
	{
	}
}