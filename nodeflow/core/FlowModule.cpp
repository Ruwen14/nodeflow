#include "nodeflow/core/FlowModule.hpp"

#include <algorithm>
namespace nf
{
void FlowModule::setModuleName(const std::string& name)
{
    m_moduleName = name;
}

std::string FlowModule::moduleName() const
{
    return m_moduleName;
}

std::set<std::string> FlowModule::registered() const
{
    std::set<std::string> ret;
    std::transform(m_flowNodeCreators.begin(),
                   m_flowNodeCreators.end(),
                   std::inserter(ret, ret.end()),
                   [](auto pair) { return pair.first; });
    std::transform(m_dataNodeCreators.begin(),
                   m_dataNodeCreators.end(),
                   std::inserter(ret, ret.end()),
                   [](auto pair) { return pair.first; });

    return ret;
}

const nf::FlowModule::FlowNodeRegistry& FlowModule::nodeCreators() const
{
    return m_flowNodeCreators;
}

const nf::FlowModule::DataNodeRegistry& FlowModule::dataCreators() const
{
    return m_dataNodeCreators;
}

nf::Expected<std::pair<std::string, std::string>, nf::RegisterError> FlowModule::
    helperParseNamePath(const std::string& namePath) const
{
    if (namePath.empty())
        return make_unexpected(RegisterError::EmptyNamePath);

    if (namePath.back() == '/')
        return make_unexpected(RegisterError::InvalidNamePath);

    if (m_flowNodeCreators.contains(namePath) || m_dataNodeCreators.contains(namePath))
        return make_unexpected(RegisterError::NameAlreadyRegistered);

    auto found = namePath.find_last_of("/\\");
    if (found == std::string::npos)
        return std::make_pair(namePath.substr(found + 1), "");

    return std::make_pair(namePath.substr(found + 1), namePath.substr(0, found));
}
} // namespace nf