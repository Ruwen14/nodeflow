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

#include "core/Node.hpp"

namespace nf
{
	enum class FlowDirection
	{
		Before,
		Next
	};

	class FlowNode : public Node
	{
	public:
		NF_NODE_NAME("FlowNode");

	public:
		NodeArchetype getArchetype() const override;

		bool onEvent(FlowEvent* event) override;

		inline void setExecNext(FlowNode& next) { m_outExecPort.execLink.makeLink(&next); forceNextExec(next); }

		inline void setExecBefore(FlowNode& before)  {  m_inExecPort.execLink.makeLink(&before); }

		inline void forceNextExec(FlowNode& next) { m_nextExec = &next; }

		inline FlowNode* getExecNext() const noexcept { return m_outExecPort.execLink.targetNode; }

		inline FlowNode* getExecBefore() const noexcept { return m_inExecPort.execLink.targetNode; }

		void breakFlow(FlowDirection dir);


	public:
		FlowPort& defaultFlowPort(FlowDirection dir);

		bool hasAdditionalFlowPorts() const;

		virtual std::vector<FlowPort*> additionalFlowPorts() const;

		virtual std::string flowPortName(FlowDirection dir, PortIndex index) const;

	private:
		FlowPort m_inExecPort;
		FlowPort m_outExecPort;
		// Optional. Used when we have multiple Output-FlowLinks
		// and need to change which node is executed next during execution (ex. Branches, Loops)
		FlowNode* m_nextExec; 
// 		FlowLink m_nextExec;
	};
}


