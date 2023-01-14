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
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>


#include "typedefs.hpp"
#include "type_tricks.hpp"
#include "../reflection/type_reflection.hpp"
#include "core/DataHandle.hpp"

namespace nf
{
	class Node;
	class FlowNode;


	enum class PortDirection
	{
		Input,
		Output,
		Both
	};

	enum class PortType
	{
		DataPort,
		ExecutionPort,
		UnconnectedPort
	};

	enum class PortCapacity
	{
		None,
		Single,
		Multi
	};

	template<typename T>
	class Port
	{
		friend Node;
		static_assert(!std::is_reference_v<T>, "Node can't have ports of reference type");
	public:
		using type_t = T;
		static constexpr auto typeID = type_id<T>();
		static constexpr auto streamable = has_ostream_operator_v<T>;

		inline operator PortIndex() const { return m_portIndex; }


		PortIndex index() const noexcept
		{
			return m_portIndex;
		}

		bool assigned() const noexcept
		{
			return m_portIndex != -1;
		}

	private:
		void setIndex(PortIndex index)
		{
			m_portIndex = index;
		}

	public:
		PortIndex m_portIndex = -1;
	};


	template<typename T>
	class InputPort : public Port<T>
	{
		friend Node;
		static_assert(std::is_same_v<T, void> != true, "Node can't have input of type <void>");
	};


	template<typename T>
	class OutputPort : public Port<T>
	{
		friend Node;
	public:
		OutputPort() = default;
		explicit OutputPort(const T& defaultValue)
			: value(defaultValue)
		{}


		template<typename T>
		void setValue(T&& val)
		{
// 			pprint(nf::type_name<decltype(val)>());
			value = std::forward<T>(val);
		}

		bool serialize(std::stringstream& archive) const 
		{
			if constexpr (this->streamable) {
				archive << value;
				return true;
			}
			else
				return false;
		}

		bool deserialize(std::stringstream& archive)
		{
			if constexpr (has_istream_operator_v<T>) {
				archive >> value;
				return true;
			}
			else
				return false;
		}

	public:
		T value{};
	};

	template<>
	class OutputPort<void> : public Port<void>
	{

	};


	template<typename T>
	struct is_input : std::false_type {};

	template<typename T>
	struct is_input<nf::InputPort<T>> : std::true_type {};

	struct PortLink
	{
		PortLink() = default;
		PortLink(PortIndex targetIndex_, Node* targetNode_)
			: targetIndex(targetIndex_), targetNode(targetNode_)
		{}

		inline bool valid() const noexcept { return targetNode != nullptr && targetIndex != -1;}
			
		void breakLink() noexcept;
		
		void setTarget(PortIndex targetIndex_, Node* targetNode_) noexcept;

		bool operator==(const PortLink& rhs) const;
			
		friend std::ostream& operator<< (std::ostream& stream, const PortLink& link)
		{
			stream << "{port=" << link.targetIndex << ", node=" << link.targetNode << "}";
			return stream;
		}

		PortIndex targetIndex = -1;
		Node* targetNode = nullptr;
	};


	template<class... Ts>
	struct ExpandInputPorts
	{
		using value = std::tuple<InputPort<Ts>...>;
	};

	template<class... Ts>
	struct ExpandInputPorts < std::tuple<Ts...>>
	{
		using value = std::tuple<InputPort<Ts>...>;
	};


	template<class... Ts>
	struct ExpandOutputPorts
	{
		using value = std::tuple<OutputPort<Ts>...>;
	};

	template<class... Ts>
	struct ExpandOutputPorts < std::tuple<Ts...>>
	{
		using value = std::tuple<OutputPort<Ts>...>;
	};




	struct FlowLink
	{
		FlowLink() = default;
		FlowLink(FlowNode* targetNode_)
			: targetNode(targetNode_)
		{}

		inline bool valid() const noexcept { return targetNode != nullptr; }

		void breakLink() noexcept;

		void setTarget(FlowNode* targetNode_) noexcept;

		bool operator==(const FlowLink& rhs) const;

		friend std::ostream& operator<< (std::ostream& stream, const FlowLink& link)
		{
			stream << "FlowLink{targetNode=" << link.targetNode << "}";
			return stream;
		}

		FlowNode* targetNode = nullptr;
	};


	class InputPortHandle
	{
		friend Node;
	public:
		InputPortHandle() = default;
		InputPortHandle( typeid_t typeID, const std::string& caption = "")
			: m_name(caption), m_link(-1, nullptr), m_dataTypeID(typeID)
		{}

		bool makeLink(PortIndex targetIndex, Node* targetNode);

		bool hasValidLink() const noexcept;

		void breakLink() noexcept;

		std::string name() const noexcept { return m_name; }

		void setName(const std::string& name) { m_name = name; }

		PortLink link() const noexcept { return m_link; }
		
		inline typeid_t typeID() const noexcept { return m_dataTypeID; }

	private:
		std::string m_name;
		PortLink m_link;
		typeid_t m_dataTypeID;
	};

	class OutputPortHandle
	{
		friend Node;
	public:
		OutputPortHandle() = default;

		template<typename T>
		OutputPortHandle(T& data, typeid_t typeID, const std::string& caption = "")
			: m_name(caption), m_dataHandle(data, typeID)
		{}

		bool createLink(PortIndex targetIndex, Node* targetNode);

		bool removeLink(PortLink link);

		bool hasLink(PortLink link) const;

		size_t linkCount() const noexcept;

		void breakAllLinks();

		std::string name() const noexcept { return m_name; }

		void setName(const std::string& name) { m_name = name; }

		const std::vector<PortLink>& links() const noexcept { return m_links; }

		template<typename T>
		void setDataHandle(T& data, typeid_t typeID)
		{
			m_dataHandle.reset();
			m_dataHandle.assign(data, typeID);
		}

		inline const detail::DataHandle& dataHandle() const { return m_dataHandle; }

		inline typeid_t typeID() const noexcept { return m_dataHandle.typeID(); }

	private:
		std::string m_name;
		std::vector<PortLink> m_links; // Output link to multiple nodes
		detail::DataHandle m_dataHandle;
	};


}