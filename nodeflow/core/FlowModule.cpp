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
    std::transform(m_flowNodeRegistry.begin(),
                   m_flowNodeRegistry.end(),
                   std::inserter(ret, ret.end()),
                   [](auto pair) { return pair.first; });
    std::transform(m_dataNodeRegistry.begin(),
                   m_dataNodeRegistry.end(),
                   std::inserter(ret, ret.end()),
                   [](auto pair) { return pair.first; });

    return ret;
}

const FlowModule::FlowNodeRegistry& FlowModule::nodeRegistry() const
{
    return m_flowNodeRegistry;
}

const FlowModule::DataNodeRegistry& FlowModule::variableRegistry() const
{
    return m_dataNodeRegistry;
}

nf::Expected<std::pair<std::string, std::string>, nf::RegisterError> FlowModule::
    helperParseNamePath(const std::string& namePath) const
{
    if (namePath.empty())
        return make_unexpected(RegisterError::EmptyNamePath);

    if (namePath.back() == '/')
        return make_unexpected(RegisterError::InvalidNamePath);

    if (m_flowNodeRegistry.contains(namePath) || m_dataNodeRegistry.contains(namePath))
        return make_unexpected(RegisterError::NameAlreadyRegistered);

    auto found = namePath.find_last_of("/\\");
    if (found == std::string::npos)
        return std::make_pair(namePath.substr(found + 1), "");

    return std::make_pair(namePath.substr(found + 1), namePath.substr(0, found));
}
} // namespace nf