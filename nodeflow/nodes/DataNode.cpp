#include "DataNode.hpp"

namespace nf
{
std::string DataNode::nodeName() const
{
    return "DataNode";
}

NodeArchetype DataNode::getArchetype() const
{
    return NodeArchetype::DataNode;
}
} // namespace nf