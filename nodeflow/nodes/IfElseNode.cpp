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
    auto cond = getInput(m_condition);

    if (cond == nullptr)
        return;
}

void IfElseNode::setExecFlowIf(FlowNode& node)
{
    NF_UNUSED(node);
}

void IfElseNode::setExecFlowElse(FlowNode& node)
{
    NF_UNUSED(node);
}
} // namespace nf