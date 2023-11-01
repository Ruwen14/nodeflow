/*
- nodeflow -
BSD 3-Clause License

Copyright (c) 2022, Ruwen Kohm
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "nodeflow/core/Error.hpp"
#include "nodeflow/core/FlowEvent.hpp"
#include "nodeflow/core/NodePort.hpp"
#include "nodeflow/core/UUID.hpp"
#include "nodeflow/typedefs.hpp"
#include "nodeflow/utility/TypenameAtlas.hpp"

namespace nf
{
enum class NodeArchetype
{
    Node,
    DataNode,
    FlowNode,
    Flow_EventNode,
    Flow_ConversionNode,
    Flow_FunctorNode,
    Flow_CustomNode,
    Lang_IfElse
};

enum class ConnectionError
{
    PortAlreadyLinked,
    PortIndexInvalid,
    UnknownNode,
    UnequalPortTypes,
    ConnectionWithItself
};

enum class StreamFlag
{
    WriteTo,
    ReadFrom
};

class Node
{
public:
    Node() = default;
    virtual ~Node() = default;

public: // Or better private and friend FlowScript
    /**
     * @brief Returns the name of the node
     */
    virtual std::string nodeName() const = 0;

    virtual typeid_t nodeType() const = 0;

    /**
     * @brief Returns the name of a specific port attached to this node.
     * Behaviour is implemented in derived classes
     */
    virtual std::string portName(PortDirection dir, PortIndex index) const;

    /**
     * @brief Returns the archetype of a node and therefore describes its
     * behaviour. Users should not override and can ignore this function.
     * @return
     */
    virtual NodeArchetype getArchetype() const;

    /**
     * @brief Allows the de/serialization of node output ports depending on the
     * StreamFlag. Useful if you want to read the outputs of nodes in the GUI or
     * set them via widgets. Supports all data types that have the ostream
     * operator<< and istream operator>> implemented.
     * @return 'true' if stream operation was successfull / is supported by
     * type.
     */
    virtual bool streamOutput(PortIndex index, StreamFlag flag, std::stringstream& archive);

    /**
     * @brief Allows to set value of a node output specifiec by index by string.
     * Only works when port type supports istream operator>>
     * @return 'true' if operation was successfull. 'false' if not (for example
     * when missing istream operator>>).
     */
    [[nodiscard]] virtual bool setOutputFromString(PortIndex index, const std::string& val);

    /**
     * @brief Gets output of node specified by index as string for debugging
     * purposes. Only works when port type supports ostream operator<<
     * @return value as 'std::string' if possible. 'std::nullopt' if not (for
     * example when missing ostream operator<<).
     */
    virtual std::optional<std::string> getOutputAsString(PortIndex index);

    /**
     * @brief Users can override to react on their custom events emitted from
     * FlowScript
     * @param event
     * @return
     */
    virtual bool onEvent(FlowEvent* event)
    {
        NF_UNUSED(event);
        return false;
    }

    /**
     * @brief Called when node is about to be removed/deleted from a FlowScript.
     * Might be used to do clean up stuff.
     * Behaviour can be specified in derived classes.
     * @return
     */
    virtual void onDestroy()
    {
    }

    // 	protected:
public:
    /**
     * @brief Is called during the construction of a node.
     * Users should add their ports and perform other initializations in the
     * derived version.
     * @return nothing or a user-defined error according to nf::Error that can
     * propagate to the gui
     */

    // ToDo Maybe pass a NodeMetaContext to setup, where its name, portnames...
    // can be decicided.
    virtual ErrorOr<void> setup()
    {
        return {};
    }

    /**
     * @brief  Workhorse function of a node in which a user-defined calculation
     * can happen. Called for each node during FlowScript::run(). Behaviour can
     * be specified in derived classes.
     */
    virtual void process()
    {
    }

public:
    /**
     * @brief Returns the unique id associated with this node
     * @return UUID
     */
    inline UUID uuid() const noexcept
    {
        return m_uuid;
    };

    void setUUID(UUID uuid) noexcept;

    /**
     * @brief Returns the number of ports used in the node
     * @param dir Input or Output
     */
    size_t portCount(PortDirection dir) const noexcept;

    /**
     * @brief Returns all input ports of the node
     */
    const std::vector<InputPortHandle>& getInputPortList() const noexcept;

    /**
     * @brief Returns all output ports of the node
     */
    const std::vector<OutputPortHandle>& getOutputPortList() const noexcept;

    /**
     * @brief Returns pointer to the input port of node specified by 'index'
     * @return nullptr if port does not exist
     */
    const InputPortHandle* getInputPort(PortIndex index) const;

    /**
     * @brief Returns pointer to the output port of node specified by 'index'
     * @return nullptr if port does not exist
     */
    const OutputPortHandle* getOutputPort(PortIndex index) const;

    /**
     * @brief Returns pointer to the input port of node specified by 'ma,e'
     * @return nullptr if port does not exist or name was invalid
     */
    const InputPortHandle* getInputPort(const std::string& name) const;

    /**
     * @brief Returns pointer to the output port of node specified by 'name'
     * @return nullptr if port does not exist or name was invalid
     */
    const OutputPortHandle* getOutputPort(const std::string& name) const;

    void formatLinkageTree(std::ostringstream& stream) const;

    /**
     * @brief Creates a data connection between output of this node to input of
     * another'
     * @return nothing or a ConnectionError
     */
    [[nodiscard]] Expected<void, ConnectionError> makeConnection(PortIndex fromOutput,
                                                                 Node& toNode,
                                                                 PortIndex toInput,
                                                                 bool interlink = true);

    /**
     * @brief Deletes an existing data connection between output of this nodes
     * to another'
     * @return 'true' is connection was successfully removed
     */
    [[nodiscard]] bool breakConnection(PortIndex fromOutput, Node& toNode, PortIndex toInput);

    /**
     * @brief Deletes all port connections from and to this node
     * @param dir Input, Output
     */
    void breakAllConnections(PortDirection dir);

protected:
    /**
     * @brief Adds an InputPort of type 'T' to the node.
     * The corresponding port must live as a member in the derived class.
     * @tparam T datatype of node.
     * @param p InputPort<T>
     * @return 'false' if port was already added.
     */
    template <typename T>
    bool addPort(InputPort<T>& p, const std::string& caption = "");

    /**
     * @brief Adds an Output of type 'T' to the node.
     * The corresponding port must live as a member in the derived class.
     * @tparam T datatype of node.
     * @param p Output<T>
     * @return 'false' if port was already added.
     */
    template <typename T>
    bool addPort(OutputPort<T>& p, const std::string& caption = "");

    /**
     * @brief Retrieve the data of the output port of a node connected to this
     * port.
     * @tparam T datatype
     * @param p InputPort<T>
     * @return nullptr if there is no connection to this input port
     */
    template <typename T>
    const T* getInput(const InputPort<T>& p) const
    {
        const PortLink& link = m_inputPorts[p.m_portIndex].m_link;
        // No input connection
        if (!link.valid())
            return nullptr;

        const detail::DataHandle& datahandle =
            link.targetNode->m_outputPorts[link.targetIndex].m_dataHandle;
        return datahandle.get<T>();
    }

    /**
     * @brief Retrieve the mutable data of the output port of a node connected
     * to this port.
     * @tparam T datatype
     * @param p InputPort<T>
     * @return nullptr if there is no connection to this input port
     */
    template <typename T>
    T* getInputMutable(const InputPort<T>& p) const
    {
        const PortLink& link = m_inputPorts[p.m_portIndex].m_link;
        // No input connection
        if (!link.valid())
            return nullptr;

        detail::DataHandle& datahandle =
            link.targetNode->m_outputPorts[link.targetIndex].m_dataHandle;
        return datahandle.getMutable<T>();
    }

    /**
     * @brief Set the output data of the node port
     */
    template <typename T, typename Ty>
    void setOutput(OutputPort<T>& p, Ty&& val) noexcept
    {
        // 			pprint(nf::type_name<decltype(val)>());
        p.setValue(std::forward<Ty>(val));
    }

    /**
     * @brief Reserves the necessary memory to store 'size' 'dir' ports.
     * Useful to save memory on nodes with few ports.
     * Should be called before ports are added in 'setup' function
     */
    void allocateExpectedPortCount(PortDirection dir, size_t size);

protected:
    std::vector<OutputPortHandle> m_outputPorts;
    std::vector<InputPortHandle> m_inputPorts;
    UUID m_uuid;
};

template <typename T>
bool Node::addPort(InputPort<T>& p, const std::string& caption /*= ""*/)
{
    if (p.assigned())
        return false;
    m_inputPorts.emplace_back(p.typeID, caption);
    p.setIndex(static_cast<int>(m_inputPorts.size() - 1));

    auto& atlas = TypenameAtlas::instance();
    atlas.add<T>();

    return true;
}

template <typename T>
bool Node::addPort(OutputPort<T>& p, const std::string& caption /*= ""*/)
{
    if (p.assigned())
        return false;
    m_outputPorts.emplace_back(p.value, p.typeID, caption);
    p.setIndex(static_cast<int>(m_outputPorts.size() - 1));

    auto& atlas = TypenameAtlas::instance();
    atlas.add<T>();

    return true;
}

// 	/**
// 	 * @brief Checks if the node can be explicitly cast to 'To'-NodeType.
// 	 * Doesn't account for inheritance like dynamic_cast.
// 	 * @tparam To
// 	 * @param from
// 	 * @return
// 	*/
// 	template<typename To>
// 	To* explicit_node_cast(Node* from)
// 	{
// 		static constexpr typeid_t toTypeID = type_id<To>();
// 		if (from && from->typeID() == toTypeID)
// 		{
// 			NF_ASSERT(dynamic_cast<To*>(from) != nullptr, "Error");
// 			return static_cast<To*>(from);
// 		}
// 		return nullptr;
// 	}
} // namespace nf