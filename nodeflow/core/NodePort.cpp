#include "NodePort.hpp"


namespace nf
{
#pragma region PortLink

	void PortLink::unlink() noexcept
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

	void ExecutionLink::breakLink() noexcept
	{
		targetNode = nullptr;
	}

	void ExecutionLink::makeLink(FlowNode* targetNode_) noexcept
	{
		targetNode = targetNode_;
	}

	bool ExecutionLink::operator==(const ExecutionLink& rhs) const
	{
		return targetNode == rhs.targetNode;
	}

#pragma endregion FlowLink

#pragma region InputPortHandle

	bool InputPortHandle::createLink(PortLink link)
	{
		if (!link.valid())
		{
			NF_ASSERT(false, "Invalid Link");
			return false;
		}
		m_link = link;
		NF_ASSERT(m_link == link, "ERROR");
		return true;
	}

	bool InputPortHandle::hasValidLink() const noexcept
	{
		return m_link.valid();
	}

	void InputPortHandle::removeLink() noexcept
	{
		m_link.unlink();
	}

#pragma endregion InputPortHandle

#pragma region OutputPortHandle

	bool OutputPortHandle::createLink(PortLink link)
	{
		if (!link.valid())
		{
			NF_ASSERT(false, "Invalid Link");
			return false;
		}
		m_links.push_back(link);
		return true;
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


