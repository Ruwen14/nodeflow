#include "nodes/EventNode.hpp"

namespace nf 
{

	StartEventNode::StartEventNode()
	{
		allocateExpectedPortCount(PortDirection::Input, 0);
		allocateExpectedPortCount(PortDirection::Output, 0);
	}

	std::string StartEventNode::nodeName() const
	{
		return "StartEventNode";
	}

	Expected<void, Error> StartEventNode::setup()
	{
		return {};
	}

	void StartEventNode::process()
	{

	}

	NodeArchetype StartEventNode::getArchetype() const
	{
		return NodeArchetype::Flow_EventNode;
	}

	
}

