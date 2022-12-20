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
	class NFNode;


	enum class PortDirection
	{
		Input,
		Output,
		None
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
		friend NFNode;
		static_assert(!std::is_reference_v<T>, "Node can't have ports of reference type");
	public:
		using type_t = T;
		static constexpr auto typeID = type_id<T>();
		static constexpr auto streamable = has_ostream_operator_v<T>;
		static constexpr auto PORT_DEBUG_NAME = type_name<T>();

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

	private:
		PortIndex m_portIndex = -1;
	};


	template<typename T>
	class InputPort : public Port<T>
	{
		friend NFNode;
		static_assert(std::is_same_v<T, void> != true, "Node can't have input of type <void>");
	};


	template<typename T>
	class OutputPort : public Port<T>
	{
		friend NFNode;
	public:
		OutputPort() = default;
		explicit OutputPort(const T& defaultValue)
			: m_value(defaultValue)
		{}


		template<typename T>
		void setValue(T&& val)
		{
			pprint(nf::type_name<decltype(val)>());
			m_value = std::forward<T>(val);
		}

		bool serialize(std::ostringstream& stream) const {
			if constexpr (this->streamable)
			{
				stream << m_value;
				return true;
			}
			return false;
		}

	public:
		T m_value{};
	};

	template<>
	class OutputPort<void> : public Port<void>
	{

	};

	namespace detail
	{
		struct PortLink
		{
			PortLink() = default;
			PortLink(PortIndex targetIndex_, NFNode* targetNode_)
				: targetIndex(targetIndex_), targetNode(targetNode_)
			{}

			inline bool valid() const noexcept { return targetNode != nullptr && targetIndex != -1;}
			
			void breakLink() noexcept;
		
			void setTarget(PortIndex targetIndex_, NFNode* targetNode_) noexcept;

			bool operator==(const PortLink& rhs) const;
			
			PortIndex targetIndex = -1;
			NFNode* targetNode = nullptr;
		};

		class InputPortContext
		{
			friend NFNode;
		public:
			InputPortContext() = default;
			InputPortContext( typeid_t typeID, const std::string& caption = "")
				: m_name(caption), m_link(-1, nullptr), m_dataTypeID(typeID)
			{}

			bool makeLink(PortIndex targetIndex, NFNode* targetNode);

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

		class OutputPortContext
		{
			friend NFNode;
		public:
			OutputPortContext() = default;

			template<typename T>
			OutputPortContext(T& data, typeid_t typeID, const std::string& caption = "")
				: m_name(caption), m_dataHandle(data, typeID)
			{}

			bool createLink(PortIndex targetIndex, NFNode* targetNode);

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

			inline const DataHandle& dataHandle() const { return m_dataHandle; }

			inline typeid_t typeID() const noexcept { return m_dataHandle.typeID(); }

		public:
			std::string m_name;
			std::vector<PortLink> m_links; // Output link to multiple nodes
			DataHandle m_dataHandle;
		};


	}
}