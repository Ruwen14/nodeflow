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
#include <vector>
#include <string>
#include <string_view>
#include <sstream>

#include "typedefs.hpp"
#include "core/NFObject.hpp"
#include "core/NodePort.hpp"

namespace nf
{
	struct LinkageResult
	{
		enum LinkageFlag
		{
			Success,
			ERROR_PortAlreadyLinked,
			ERROR_PortIndexInvalid,
			ERROR_NodeInvalid,
			ERROR_UnequalPortTypes
		};

		LinkageResult(LinkageFlag flag)
			: m_linkflag(flag)
		{}

		operator bool() const noexcept
		{
			if (m_linkflag == Success)
				return true;
			return false;
		}

		LinkageFlag result() const noexcept
		{
			return m_linkflag;
		}

	private:
		LinkageFlag m_linkflag;
	};



	class NFNode : public NFObject
	{
	public:
		NFNode() = default;
		virtual ~NFNode() = default;

	public: // Or better private and friend FlowScript
		virtual std::string nodeName() const { return "NFNode"; }

		virtual bool serialize(std::ostringstream& serializer, 
			PortDirection dir, PortIndex index) const;

// 	protected:
	public:
		virtual bool setup() { return false; }

		virtual void process() {}

	public:
		size_t portCount(PortDirection dir) const noexcept;

		const std::vector<detail::InputPortContext>& getInputPortList() const noexcept;

		const std::vector<detail::OutputPortContext>& getOutputPortList() const noexcept;

		const detail::InputPortContext& getInputPort(PortIndex index) const;

		const detail::OutputPortContext& getOutputPort(PortIndex index) const;

		const detail::InputPortContext* findInputPort(std::string_view name) const;

		const detail::OutputPortContext* findOutputPort(std::string_view name) const;

		void formatLinkageTree(std::ostringstream& stream) const;


		LinkageResult makeLink(PortIndex fromOutIndex, NFNode* toNode, PortIndex toInIndex);
	protected:

		void clearLinks(PortDirection dir);

	protected:
		template<typename T>
		bool addPort(nf::InputPort<T>& p, const std::string& caption = "");

		template<typename T>
		bool addPort(nf::OutputPort<T>& p, const std::string& caption = "");

		template<typename T>
		const T* getInputData(const nf::InputPort<T>& p) const
		{
			const detail::PortLink& link = m_inputPorts[p.m_portIndex].m_link;
			if (!link.valid())
				return nullptr;

			const detail::DataHandle& datahandle = link.targetNode->m_outputPorts[link.targetIndex].m_dataHandle;
			return datahandle.get<T>();
		}

		template<typename T, typename Ty>
		void setOutputData(nf::OutputPort<T>& p, Ty&& val) noexcept
		{
			pprint(nf::type_name<decltype(val)>());
			p.setValue(std::forward<Ty>(val));
		}
	public:
		std::vector<detail::OutputPortContext> m_outputPorts;
		std::vector<detail::InputPortContext> m_inputPorts;

	};

	template<typename T>
	bool NFNode::addPort(nf::InputPort<T>& p, const std::string& caption /*= ""*/)
	{
		if (p.assigned())
			return false;
		m_inputPorts.emplace_back(p.typeID, caption);
		p.setIndex(static_cast<int>(m_inputPorts.size() - 1));
		return true;
	}


	template<typename T>
	bool NFNode::addPort(nf::OutputPort<T>& p, const std::string& caption /*= ""*/)
	{
		if (p.assigned())
			return false;
		m_outputPorts.emplace_back(p.m_value, p.typeID, caption);
		p.setIndex(static_cast<int>(m_outputPorts.size() - 1));
		return true;
	}

}