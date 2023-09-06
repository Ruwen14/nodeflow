#include "nodeflow/utility/TypenameAtlas.hpp"

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace nf
{
	std::set<std::string> TypenameAtlas::listTypenames() const
	{
		std::set<std::string> ret;
		std::transform(m_atlas.begin(), m_atlas.end(),
			std::inserter(ret, ret.end()),
			[](auto pair) { return pair.second; });

		return ret;
	}

	bool TypenameAtlas::hasDuplicates() const
	{
		std::vector<std::string> atlasValues;
		for (const auto& atla : m_atlas)
			atlasValues.push_back(atla.second);

		if (!std::is_sorted(atlasValues.begin(), atlasValues.end()))
			std::sort(atlasValues.begin(), atlasValues.end());

		return std::adjacent_find(atlasValues.begin(), atlasValues.end()) != atlasValues.end();
	}
}