#include "DataNode.hpp"

namespace nf
{
#pragma region DataNode

	std::string DataNode::nodeName() const
	{
		return "DataNodeBase";
	}

	NodeArchetype DataNode::getArchetype() const
	{
		return NodeArchetype::DataNode;
	}

#pragma endregion DataNode
}