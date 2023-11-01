#include "nodeflow/core/FlowScript.hpp"

namespace nf
{
Node* FlowScript::findNode(NodeID uuid) const
{
    // FIXME: A data structure like unordered_map would definitely make more
    // sense here.
    //        But I'm not sure yet how to execute the nodes later. Vectors might
    //        be more performant then.
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

std::vector<Node*> FlowScript::findNodeByType(typeid_t type) const
{
    std::vector<Node*> results;
    for (const auto& node : m_callablesNodes)
    {
        if (node->nodeType() == type)
            results.push_back(node.get());
    }

    for (const auto& node : m_variableNodes)
    {
        if (node->nodeType() == type)
            results.push_back(node.get());
    }
    return results;
}

bool FlowScript::hasNode(NodeID uuid) const
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

std::vector<NodeID> FlowScript::nodeHandles() const
{
    std::vector<NodeID> nodes;
    nodes.reserve(m_variableNodes.size() + m_callablesNodes.size());

    for (const auto& node : m_callablesNodes)
        nodes.push_back(node->uuid());
    for (const auto& node : m_variableNodes)
        nodes.push_back(node->uuid());

    return nodes;
}

nf::ErrorOr<Node*> FlowScript::spawnNode(const std::string& namePath)
{
    auto& nodeRegistry = m_scriptModule->nodeRegistry();
    auto& varRegistry = m_scriptModule->variableRegistry();

    if (nodeRegistry.contains(namePath))
        return createNode(namePath);

    if (varRegistry.contains(namePath))
        return createVariable(namePath);

    return make_unexpected(Error("Unknown namePath during 'FlowScript::spawnNode'"));
}

ErrorOr<DataNode*> FlowScript::spawnVariable(const std::string& typeName)
{
    auto& varRegistry = m_scriptModule->variableRegistry();

    if (!varRegistry.contains(std::string(typeName)))
        return make_unexpected(Error("Variable of type 'typeName' is not registered."));

    auto instance = varRegistry.at(typeName)();

    while (!isUUIDUnique(instance->uuid()))
    {
        NF_ASSERT(false, "UUID collision found!");
        instance->setUUID(UUID::create());
    }

    if (auto setupSuccess = instance->setup(); !setupSuccess)
        return make_unexpected(setupSuccess.error());

    m_variableNodes.push_back(std::move(instance));

    return m_variableNodes[m_variableNodes.size() - 1].get();
}

bool FlowScript::removeNode(NodeID node)
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

nf::Expected<void, nf::ConnectionError> FlowScript::connectPorts(Node& fromNode,
                                                                 PortIndex fromPort,
                                                                 Node& toNode,
                                                                 PortIndex toPort)
{
    NF_ASSERT(fromNode.uuid() != toNode.uuid(), "Connection with itself not allowed.");
    if (fromNode.uuid() == toNode.uuid())
        return make_unexpected(ConnectionError::ConnectionWithItself);
    return fromNode.makeConnection(fromPort, toNode, toPort);
}

nf::Expected<void, nf::ConnectionError> FlowScript::connectPorts(NodeID fromNode,
                                                                 PortIndex fromPort,
                                                                 NodeID toNode,
                                                                 PortIndex toPort)
{
    auto outNode = findNode(fromNode);
    auto inNode = findNode(toNode);

    NF_ASSERT(inNode, "outNode doesn't exist.");
    NF_ASSERT(outNode, "outNode doesn't exist.");
    if (!outNode || !inNode)
        return make_unexpected(ConnectionError::UnknownNode);

    return connectPorts(*outNode, fromPort, *inNode, toPort);
    //     return outNode->makeConnection(fromPort, *inNode, toPort);
}

bool FlowScript::disconnectPorts(NodeID fromNode,
                                 PortIndex fromPort,
                                 NodeID toNode,
                                 PortIndex toPort)
{
    auto outNode = findNode(fromNode);
    auto inNode = findNode(toNode);

    if (!outNode || !inNode)
        return false;

    return outNode->breakConnection(fromPort, *inNode, toPort);
}

bool FlowScript::connectFlow(NodeID fromNode, NodeID toNode)
{
    auto outNode = findNode(fromNode);
    auto inNode = findNode(toNode);

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
nf::ExpectedRef<DataNode, Error> FlowScript::spawnType(const std::string&
namePath)
{
    auto& creators = m_scriptModule->dataCreators();

    if (!creators.contains(namePath))
        return make_unexpected(Error("Unknown namePath", 100));

    auto creator = creators.at(namePath);
    auto instance = creator();

    // Checks uniqueness of instance UUID. In the very unlikely event that a
    // collision has occurred, a new UUID is generated and assigned to the
instance. while (!isUUIDUnique(instance->uuid()))
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
Expected<void, ConnectionError> FlowScript::connectNodes(Node& outNode,
PortIndex outPort, Node& inNode, PortIndex inPort)
{
    return outNode.makeConnection(ConnectionPolicy::OutputToInput, outPort,
inNode, inPort);
}

bool FlowScript::disconnectNodes(Node& outNode, PortIndex outPort,
                            Node& inNode, PortIndex inPort)
{
    return outNode.breakConnection(ConnectionPolicy::OutputToInput, outPort,
inNode, inPort);
}
*/

bool FlowScript::disconnectFlow(NodeID fromNode, NodeID toNode)
{
    auto outNode = findNode(fromNode);
    auto inNode = findNode(toNode);

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

nf::Node* FlowScript::findNode(NodeID uuid, std::pair<int, size_t>& pos) const
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

nf::ErrorOr<Node*> FlowScript::createNode(const std::string& namePath)
{
    auto& creators = m_scriptModule->nodeRegistry();
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
    return m_callablesNodes[m_callablesNodes.size() - 1].get();
}

nf::ErrorOr<Node*> FlowScript::createVariable(const std::string& namePath)
{
    auto& creators = m_scriptModule->variableRegistry();
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

    return m_variableNodes[m_variableNodes.size() - 1].get();
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