#include "nodeflow/core/Node.hpp"

namespace nf
{
std::string Node::portName(PortDirection dir, PortIndex index) const
{
    NF_UNUSED(dir);
    NF_UNUSED(index);

    return "";
}

NodeArchetype Node::getArchetype() const
{
    return NodeArchetype::Node;
}

bool Node::streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive)
{
    NF_UNUSED(flag);
    NF_UNUSED(archive);
    NF_UNUSED(index);
    return false;
}

bool Node::setOutputDataFromString(PortIndex index, const std::string& val)
{
    NF_UNUSED(index);
    NF_UNUSED(val);
    return false;
}

std::optional<std::string> Node::getOutputDataAsString(PortIndex index)
{
    NF_UNUSED(index);
    return std::nullopt;
}

void Node::setUUID(UUID uuid) noexcept
{
    m_uuid = uuid;
}

size_t Node::portCount(PortDirection dir) const noexcept
{
    switch (dir)
    {
    case PortDirection::Input:
        return m_inputPorts.size();
    case PortDirection::Output:
        return m_outputPorts.size();
    default:
        return 0;
    }
}

const std::vector<InputPortHandle>& Node::getInputPortList() const noexcept
{
    return m_inputPorts;
}

const std::vector<OutputPortHandle>& Node::getOutputPortList() const noexcept
{
    return m_outputPorts;
}

const InputPortHandle* Node::getInputPort(PortIndex index) const
{
    if (index == -1 || !(index < m_inputPorts.size()))
        return nullptr;

    const auto& port = m_inputPorts[index];
    return &port;
}

const OutputPortHandle* Node::getOutputPort(PortIndex index) const
{
    if (index == -1 || !(index < m_outputPorts.size()))
        return nullptr;

    const auto& port = m_outputPorts[index];
    return &port;
}

const nf::InputPortHandle* Node::getInputPort(const std::string& name) const
{
    if (name.empty())
        return nullptr;
    for (size_t i = 0; i < m_inputPorts.size(); i++)
    {
        if (name == portName(PortDirection::Input, static_cast<PortIndex>(i)))
            return getInputPort(static_cast<PortIndex>(i));
    }
    return nullptr;
}

const nf::OutputPortHandle* Node::getOutputPort(const std::string& name) const
{
    if (name.empty())
        return nullptr;

    for (size_t i = 0; i < m_outputPorts.size(); i++)
    {
        if (name == portName(PortDirection::Output, static_cast<PortIndex>(i)))
            return getOutputPort(static_cast<PortIndex>(i));
    }
    return nullptr;
}

void Node::formatLinkageTree(std::ostringstream& stream) const
{
    stream << "LinkageTree for [Node:" << nodeName() << " @" << this << "]\n";
    stream << "{\n";
    stream << "    [Output Ports]:\n";
    for (const auto& oPort : m_outputPorts)
    {
        stream << "\t>[Port:" << oPort.name() << "] Link Count: " << oPort.linkCount() << "\n";
        for (const auto& link : oPort.links())
        {
            stream << "\t\t\t---<Link>--> ";

            if (!link.valid())
                stream << " Invalid";
            stream << "To [Port:" << link.targetNode->getInputPortList()[link.targetIndex].name()
                   << "][" << link.targetIndex << "] of [Node:" << link.targetNode->nodeName()
                   << " @" << link.targetNode << "]\n";
        }
    }
    stream << "\n    [Input Ports]:\n";
    for (const auto& iPort : m_inputPorts)
    {
        stream << "\t>[Port:" << iPort.name() << "] ---<Link>--> ";
        auto link = iPort.link();
        if (!link.valid())
            stream << "NOT ASSIGNED\n";
        else
        {
            stream << "To [Port:" << link.targetNode->getOutputPortList()[link.targetIndex].name()
                   << "][" << link.targetIndex << "] of [Node:" << link.targetNode->nodeName()
                   << " @" << link.targetNode << "]\n";
        }
    }
    stream << "}\n";
}

Expected<void, ConnectionError> Node::makeConnection(PortIndex fromOutput,
                                                     Node& toNode,
                                                     PortIndex toInput,
                                                     bool interlink /*= true*/)
{
    // Ports cannot be connected to ports of the same node, otherwise we would
    // have recursion.
    if (this == &toNode)
        return make_unexpected(ConnectionError::ConnectionWithItself);

    if (fromOutput == -1 || toInput == -1 || !(fromOutput < m_outputPorts.size())
        || !(toInput < toNode.m_inputPorts.size()))
        return make_unexpected(ConnectionError::PortIndexInvalid);

    if (toNode.m_inputPorts[toInput].link().valid())
        return make_unexpected(ConnectionError::PortAlreadyLinked);

    auto fromPortType = m_outputPorts[fromOutput].typeID();
    auto toPortType = toNode.m_inputPorts[toInput].typeID();
    if (fromPortType != toPortType)
        return make_unexpected(ConnectionError::UnequalPortTypes);
    // ----------------------------------------------------------

    auto& originPort = m_outputPorts[fromOutput];
    auto& targetPort = toNode.m_inputPorts[toInput];

    PortLink linkForward{ toInput, &toNode };
    if (originPort.hasLink(linkForward))
        return make_unexpected(ConnectionError::PortAlreadyLinked);

    originPort.createLink(linkForward);
    if (interlink)
    {
        PortLink linkBackward{ fromOutput, this };
        targetPort.createLink(linkBackward);
    }

    return {};
}

bool Node::breakConnection(PortIndex fromOutput, Node& toNode, PortIndex toInput)
{
    NF_ASSERT((fromOutput != -1 && toInput != -1), "Invalid port index");
    NF_ASSERT(fromOutput < m_outputPorts.size(), "Port index out of range");
    NF_ASSERT(toInput < toNode.m_inputPorts.size(), "Port index out of range");

    auto& originPort = m_outputPorts[fromOutput];
    auto& targetPort = toNode.m_inputPorts[toInput];

    PortLink linkToBeRemoved(toInput, &toNode);
    if (!originPort.hasLink(linkToBeRemoved))
    {
        NF_ASSERT(false, "Link doesn't exist");
        return false;
    }

    // Notify input port of linked node
    NF_ASSERT(targetPort.link().targetNode == this, "ERROR");
    targetPort.removeLink();
    originPort.removeLink(linkToBeRemoved);
    return true;
}

void Node::breakAllConnections(PortDirection dir)
{
    if (dir == PortDirection::Input)
    {
        for (size_t i = 0; i < m_inputPorts.size(); i++)
        {
            PortLink inToOutLink = m_inputPorts[i].link();
            if (!inToOutLink.valid())
                continue;
            auto success = inToOutLink.targetNode->breakConnection(
                inToOutLink.targetIndex, *this, static_cast<PortIndex>(i));
            NF_ASSERT(success, "Faild to break Connection");
        }
    }
    else
    {
        for (size_t i = 0; i < m_outputPorts.size(); i++)
        {
            auto& originPort = m_outputPorts[i];
            for (const PortLink& outToInLink : originPort.links())
            {
                if (!outToInLink.valid())
                {
                    NF_ASSERT(false, "Out-Link not valid. Why is that?");
                    continue;
                }
                auto success = breakConnection(
                    static_cast<PortIndex>(i), *outToInLink.targetNode, outToInLink.targetIndex);
                NF_ASSERT(success, "Faild to break Connection");
            }
            NF_ASSERT(originPort.linkCount() == 0, "ERROR in clearing links");
        }
    }
}

void Node::allocateExpectedPortCount(PortDirection dir, size_t size)
{
    if (dir == PortDirection::Input)
    {
        NF_ASSERT(m_inputPorts.empty(),
                  "ERROR: bufferExpectedPortCount only "
                  "valid before port initialization");
        if (size == 0)
        {
            m_inputPorts.reserve(0);
            m_inputPorts.shrink_to_fit();
            NF_ASSERT(m_inputPorts.capacity() == 0,
                      "Memory of m_inputPorts couldn't be emptied successfully");
        }
        else
            m_inputPorts.reserve(size);
    }
    else
    {
        NF_ASSERT(m_outputPorts.empty(),
                  "ERROR: bufferExpectedPortCount only "
                  "valid before port initialization");

        if (size == 0)
        {
            m_outputPorts.reserve(0);
            m_outputPorts.shrink_to_fit();
            NF_ASSERT(m_outputPorts.capacity() == 0,
                      "Memory of m_outputPorts couldn't be emptied successfully");
        }
        else
            m_outputPorts.reserve(size);
    }
}
} // namespace nf