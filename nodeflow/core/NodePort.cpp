#include "NodePort.hpp"


namespace nf
{
#pragma region PortLink

	void PortLink::breakLink() noexcept
	{
		targetIndex = -1;
		targetNode = nullptr;
	}

	void PortLink::setTarget(PortIndex targetIndex_, Node* targetNode_) noexcept
	{
		targetIndex = targetIndex_;
		targetNode = targetNode_;
	}

	bool PortLink::operator==(const PortLink& rhs) const
	{
		return ((targetIndex == rhs.targetIndex) && (targetNode == rhs.targetNode));
	}


#pragma endregion PortLink

#pragma  region FlowLink

	void FlowLink::breakLink() noexcept
	{
		targetNode = nullptr;
	}

	void FlowLink::setTarget(Node* targetNode_) noexcept
	{
		targetNode = targetNode_;
	}

	bool FlowLink::operator==(const FlowLink& rhs) const
	{
		return targetNode == rhs.targetNode;
	}

#pragma endregion FlowLink

#pragma region InputPortHandle

	bool InputPortHandle::makeLink(PortIndex targetIndex, Node* targetNode)
	{
		if (targetIndex == -1 || targetNode ==nullptr)
			return false;
		m_link.setTarget(targetIndex, targetNode);
		return true;
	}

	bool InputPortHandle::hasValidLink() const noexcept
	{
		return m_link.valid();
	}

	void InputPortHandle::breakLink() noexcept
	{
		m_link.breakLink();
	}

#pragma endregion InputPortHandle

#pragma region OutputPortHandle

	bool OutputPortHandle::createLink(PortIndex targetIndex, Node* targetNode)
	{
		PortLink link(targetIndex, targetNode);
		if (link.valid())
		{
			m_links.push_back(link);
			return true;
		}
		return false;
	}

	bool OutputPortHandle::removeLink(PortLink link)
	{
		auto iter = std::find(m_links.begin(), m_links.end(), link);
		// found
		if (iter != m_links.end())
		{
			m_links.erase(iter);
			return true;
		}
		return false;
	}

	bool OutputPortHandle::hasLink(PortLink link) const
	{
		return std::find(m_links.begin(), m_links.end(), link) != m_links.end();
	}

	size_t OutputPortHandle::linkCount() const noexcept
	{
		return m_links.size();
	}

	void OutputPortHandle::breakAllLinks()
	{
		m_links.clear();
	}

#pragma endregion OutputPortHandle

}


