#include "nodeflow/core/FlowScript.hpp"

namespace nf
{
Node* FlowScript::findNode(NodeHandle uuid) const
{
    // FIXME: A data structure like unordered_map would definitely make more sense here.
    //        But I'm not sure yet how to execute the nodes later. Vectors might be more performant
    //        then.
    for (auto& node : m_callablesNodes)
    {
        if (node->uuid() == uuid)
            return node.get();
    }

    for (auto& node : m_variableNodes)
    {
        if (node->uuid() == uuid)
            return node.get();
    }

    return nullptr;
}

bool FlowScript::hasNode(NodeHandle uuid) const
{
    return findNode(uuid) != nullptr;
}

std::vector<Node*> FlowScript::nodes() const
{
    std::vector<Node*> nodes;
    nodes.reserve(m_variableNodes.size() + m_callablesNodes.size());

    for (const auto& node : m_callablesNodes)
        nodes.push_back(node.get());
    for (const auto& node : m_variableNodes)
        nodes.push_back(node.get());

    return nodes;
}

size_t FlowScript::nodeCount() const noexcept
{
    return m_callablesNodes.size() + m_variableNodes.size();
}

std::vector<NodeHandle> FlowScript::nodeHandles() const
{
    std::vector<NodeHandle> nodes;
    nodes.reserve(m_variableNodes.size() + m_callablesNodes.size());

    for (const auto& node : m_callablesNodes)
        nodes.push_back(node->uuid());
    for (const auto& node : m_variableNodes)
        nodes.push_back(node->uuid());

    return nodes;
}

ErrorOr<NodeHandle> FlowScript::spawnNode(const std::string& namePath)
{
    auto& nodeCreators = m_scriptModule->nodeCreators();
    auto& dataCreators = m_scriptModule->dataCreators();

    if (nodeCreators.contains(namePath))
        return createNode(namePath);

    if (dataCreators.contains(namePath))
        return createVariable(namePath);

    return make_unexpected(Error("Unknown namePath during 'FlowScript::spawnNode'"));
}

bool FlowScript::removeNode(NodeHandle node)
{
    std::pair<int, size_t> pos;
    auto foundNode = findNode(node, pos);

    if (!foundNode)
        return false;

    foundNode->onDestroy();

    NodeArchetype archetype = foundNode->getArchetype();

    // We have a FlowNode. Disconnect FlowLinks
    if ((archetype != NodeArchetype::DataNode) && (archetype != NodeArchetype::Node))
    {
        NF_ASSERT(dynamic_cast<FlowNode*>(foundNode) != nullptr, "Error");
        auto flowNode = static_cast<FlowNode*>(foundNode);

        // Disconnect FLowLink from other node to this node and vice versa
        if (auto flowTo = flowNode->getExecNext())
            flowTo->breakFlow(FlowDirection::Before);
        flowNode->breakFlow(FlowDirection::Next);

        if (auto flowBefore = flowNode->getExecBefore())
            flowBefore->breakFlow(FlowDirection::Next);
        flowNode->breakFlow(FlowDirection::Before);

        NF_ASSERT(flowNode->getExecNext() == nullptr, "Error");
        NF_ASSERT(flowNode->getExecBefore() == nullptr, "Error");
    }

    // Remove all data connection from other nodes to this and vice versa
    foundNode->breakAllConnections(PortDirection::Input);
    foundNode->breakAllConnections(PortDirection::Output);

    NF_ASSERT(debugAllConnectionsRemovedTo(foundNode), "Error");

    if (pos.first == 0)
        m_callablesNodes.erase(m_callablesNodes.begin() + pos.second);
    else
        m_variableNodes.erase(m_variableNodes.begin() + pos.second);

    return true;
}

/*
ExpectedRef<FlowNode, Error> FlowScript::spawnNode(const std::string& namePath)
{
    auto& creators = m_scriptModule->nodeCreators();

    if (!creators.contains(namePath))
        return make_unexpected(Error("Unknown namePath", 100));

    auto creator = creators.at(namePath);
    auto instance = creator();

    while (!isUUIDUnique(instance->uuid()))
    {
        NF_ASSERT(false, "UUID collision found!");
        instance->setUUID(UUID::create());
    }

    if (auto setupSuccess = instance->setup(); !setupSuccess)
        return make_unexpected(setupSuccess.error());

    m_callablesNodes.push_back(std::move(instance));

    auto pos = m_callablesNodes.size() - 1;
    return std::ref(*m_callablesNodes[pos]);
}
*/

Expected<void, ConnectionError> FlowScript::connectPorts(
    NodeHandle outNodeUUID,
    PortIndex outPort,
    NodeHandle inNodeUUID,
    PortIndex inPort,
    ConversionPolicy conv /*= ConversionPolicy::DontAddConversion*/)
{
    NF_UNUSED(conv);

    auto outNode = findNode(outNodeUUID);
    auto inNode = findNode(inNodeUUID);

    if (!outNode || !inNode)
        return make_unexpected(ConnectionError::UnknownNode);

    return outNode->makeConnection(outPort, *inNode, inPort);
}

bool FlowScript::disconnectPorts(NodeHandle outNodeUUID,
                                 PortIndex outPort,
                                 NodeHandle inNodeUUID,
                                 PortIndex inPort)
{
    auto outNode = findNode(outNodeUUID);
    auto inNode = findNode(inNodeUUID);

    if (!outNode || !inNode)
        return false;

    return outNode->breakConnection(outPort, *inNode, inPort);
}

bool FlowScript::connectFlow(NodeHandle outNodeUUID, NodeHandle inNodeUUID)
{
    auto outNode = findNode(outNodeUUID);
    auto inNode = findNode(inNodeUUID);

    if (!outNode || !inNode)
        return false;

    NodeArchetype outNodeType = outNode->getArchetype();
    NodeArchetype inNodeType = inNode->getArchetype();

    // Only FlowNodes and their childs can have flow links
    if (outNodeType == NodeArchetype::DataNode || outNodeType == NodeArchetype::Node
        || inNodeType == NodeArchetype::DataNode || inNodeType == NodeArchetype::Node)
        return false;

    NF_ASSERT(dynamic_cast<FlowNode*>(outNode) != nullptr, "Well we have a problem");
    NF_ASSERT(dynamic_cast<FlowNode*>(inNode) != nullptr, "Well we have a problem");

    auto outFlowNode = static_cast<FlowNode*>(outNode);
    auto inFlowNode = static_cast<FlowNode*>(inNode);

    outFlowNode->setExecNext(*inFlowNode);
    inFlowNode->setExecBefore(*outFlowNode);

    NF_ASSERT(outFlowNode->getExecNext() != nullptr, "Error");
    NF_ASSERT(inFlowNode->getExecBefore() != nullptr, "Error");

    return true;
}

/*
nf::ExpectedRef<DataNode, Error> FlowScript::spawnType(const std::string& namePath)
{
    auto& creators = m_scriptModule->dataCreators();

    if (!creators.contains(namePath))
        return make_unexpected(Error("Unknown namePath", 100));

    auto creator = creators.at(namePath);
    auto instance = creator();

    // Checks uniqueness of instance UUID. In the very unlikely event that a
    // collision has occurred, a new UUID is generated and assigned to the instance.
    while (!isUUIDUnique(instance->uuid()))
    {
        NF_ASSERT(false, "UUID collision found!");
        instance->setUUID(UUID::create());
    }

    if (auto setupSuccess = instance->setup(); !setupSuccess)
        return make_unexpected(setupSuccess.error());

    m_variableNodes.push_back(std::move(instance));
    auto pos = m_variableNodes.size() - 1;
    return std::ref(*m_variableNodes[pos]);
}
*/

/*
Expected<void, ConnectionError> FlowScript::connectNodes(Node& outNode, PortIndex outPort,
                                                    Node& inNode, PortIndex inPort)
{
    return outNode.makeConnection(ConnectionPolicy::OutputToInput, outPort, inNode, inPort);
}

bool FlowScript::disconnectNodes(Node& outNode, PortIndex outPort,
                            Node& inNode, PortIndex inPort)
{
    return outNode.breakConnection(ConnectionPolicy::OutputToInput, outPort, inNode, inPort);
}
*/

bool FlowScript::disconnectFlow(NodeHandle outNodeUUID, NodeHandle inNodeUUID)
{
    auto outNode = findNode(outNodeUUID);
    auto inNode = findNode(inNodeUUID);

    if (!outNode || !inNode)
        return false;

    NodeArchetype outNodeType = outNode->getArchetype();
    NodeArchetype inNodeType = inNode->getArchetype();

    // Only FlowNodes and their childs can have flow links
    if (outNodeType == NodeArchetype::DataNode || outNodeType == NodeArchetype::Node
        || inNodeType == NodeArchetype::DataNode || inNodeType == NodeArchetype::Node)
        return false;

    NF_ASSERT(dynamic_cast<FlowNode*>(outNode) != nullptr, "Well we have a problem");
    NF_ASSERT(dynamic_cast<FlowNode*>(inNode) != nullptr, "Well we have a problem");

    auto outFlowNode = static_cast<FlowNode*>(outNode);
    auto inFlowNode = static_cast<FlowNode*>(inNode);

    inFlowNode->breakFlow(FlowDirection::Before);
    outFlowNode->breakFlow(FlowDirection::Next);

    NF_ASSERT(inFlowNode->getExecBefore() == nullptr, "Error");
    NF_ASSERT(outFlowNode->getExecNext() == nullptr, "Error");

    return true;
}

nf::Node* FlowScript::findNode(NodeHandle uuid, std::pair<int, size_t>& pos) const
{
    for (size_t i = 0; i < m_callablesNodes.size(); i++)
    {
        if (m_callablesNodes[i]->uuid() == uuid)
        {
            pos.first = 0;
            pos.second = i;
            return m_callablesNodes[i].get();
        }
    }

    for (size_t j = 0; j < m_variableNodes.size(); j++)
    {
        if (m_variableNodes[j]->uuid() == uuid)
        {
            pos.first = 1;
            pos.second = j;
            return m_variableNodes[j].get();
        }
    }

    return nullptr;
}

nf::Node* FlowScript::findPortConversionNode(typeid_t fromType, typeid_t toType) const
{
    for (auto& node : m_callablesNodes)
    {
        if (node->getArchetype() == NodeArchetype::Flow_ConversionNode)
        {
            NF_ASSERT(dynamic_cast<ConversionNode*>(node.get()) != nullptr, "Error");
            auto conversion = static_cast<ConversionNode*>(node.get());
            // Found
            if (conversion->from() == fromType && conversion->to() == toType)
                return node.get();
        }
    }
    return nullptr;
}

bool FlowScript::isUUIDUnique(UUID uuid) const
{
    for (const auto& node : m_callablesNodes)
    {
        if (node->uuid() == uuid)
            return false;
    }

    for (const auto& node : m_variableNodes)
    {
        if (node->uuid() == uuid)
            return false;
    }
    return true;
}

ErrorOr<NodeHandle> FlowScript::createNode(const std::string& namePath)
{
    auto& creators = m_scriptModule->nodeCreators();
    auto creator = creators.at(namePath);
    auto instance = creator();

    while (!isUUIDUnique(instance->uuid()))
    {
        NF_ASSERT(false, "UUID collision found!");
        instance->setUUID(UUID::create());
    }

    if (auto setupSuccess = instance->setup(); !setupSuccess)
        return make_unexpected(setupSuccess.error());

    m_callablesNodes.push_back(std::move(instance));

    return m_callablesNodes[m_callablesNodes.size() - 1]->uuid();
}

ErrorOr<NodeHandle> FlowScript::createVariable(const std::string& namePath)
{
    auto& creators = m_scriptModule->dataCreators();
    auto creator = creators.at(namePath);
    auto instance = creator();

    while (!isUUIDUnique(instance->uuid()))
    {
        NF_ASSERT(false, "UUID collision found!");
        instance->setUUID(UUID::create());
    }

    if (auto setupSuccess = instance->setup(); !setupSuccess)
        return make_unexpected(setupSuccess.error());

    m_variableNodes.push_back(std::move(instance));

    return m_variableNodes[m_variableNodes.size() - 1]->uuid();
}

bool FlowScript::debugAllConnectionsRemovedTo(nf::Node* node) const
{
    for (const auto& cNode : m_callablesNodes)
    {
        for (const auto& iPort : cNode->getInputPortList())
        {
            if (iPort.link().targetNode == node)
                return false;
        }
        for (const auto& oPort : cNode->getOutputPortList())
        {
            for (const auto& oLink : oPort.links())
            {
                if (oLink.targetNode == node)
                    return false;
            }
        }
    }

    for (const auto& vNode : m_variableNodes)
    {
        for (const auto& iPort : vNode->getInputPortList())
        {
            if (iPort.link().targetNode == node)
                return false;
        }
        for (const auto& oPort : vNode->getOutputPortList())
        {
            for (const auto& oLink : oPort.links())
            {
                if (oLink.targetNode == node)
                    return false;
            }
        }
    }
    return true;
}
} // namespace nf