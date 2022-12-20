#include "NodePort.hpp"

namespace nf::detail
{
#pragma region PortLink

	void PortLink::breakLink() noexcept
	{
		targetIndex = -1;
		targetNode = nullptr;
	}

	void PortLink::setTarget(PortIndex targetIndex_, NFNode* targetNode_) noexcept
	{
		targetIndex = targetIndex_;
		targetNode = targetNode_;
	}

	bool PortLink::operator==(const PortLink& rhs) const
	{
		return ((targetIndex == rhs.targetIndex) && (targetNode == rhs.targetNode));
	}


#pragma endregion PortLink

#pragma region InputPortContext

	bool InputPortContext::makeLink(PortIndex targetIndex, NFNode* targetNode)
	{
		if (targetIndex == -1 || targetNode ==nullptr)
			return false;
		m_link.setTarget(targetIndex, targetNode);
		return true;
	}

	bool InputPortContext::hasValidLink() const noexcept
	{
		return m_link.valid();
	}

	void InputPortContext::breakLink() noexcept
	{
		m_link.breakLink();
	}

#pragma endregion InputPortContext

#pragma region OutputPortContext

	bool OutputPortContext::createLink(PortIndex targetIndex, NFNode* targetNode)
	{
		PortLink link(targetIndex, targetNode);
		if (link.valid())
		{
			m_links.push_back(link);
			return true;
		}
		return false;
	}

	size_t OutputPortContext::linkCount() const noexcept
	{
		return m_links.size();
	}

	void OutputPortContext::breakAllLinks()
	{
		m_links.clear();
	}

#pragma endregion OutputPortContext

}


