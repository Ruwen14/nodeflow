#include "nodeflow/nodes/ConversionNode.hpp"

namespace nf
{
std::string ConversionNode::nodeName() const
{
    return "ConversionNode";
}

NodeArchetype ConversionNode::getArchetype() const
{
    return NodeArchetype::Flow_ConversionNode;
}

typeid_t ConversionNode::from() const
{
    NF_ASSERT(m_inputPorts.size() == 1, "Error");
    return m_inputPorts.at(0).typeID();
}

typeid_t ConversionNode::to() const
{
    NF_ASSERT(m_outputPorts.size() == 1, "Error");
    return m_outputPorts.at(0).typeID();
}
} // namespace nf