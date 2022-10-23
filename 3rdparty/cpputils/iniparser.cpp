#include "iniparser.h"

namespace cpputils
{
	std::set<std::string> IniConfig::keys(const std::string& sect) const
	{
		std::set<std::string> keyset;
		for (const auto& key : this->sections_.at(sect))
			keyset.insert(key.first);
		return keyset;
	}

	std::set<std::string> IniConfig::sections() const
	{
		std::set<std::string> sectionset;
		for (const auto& sec : this->sections_)
			sectionset.insert(sec.first);
		return sectionset;
	}

	bool IniConfig::hasKey(const std::string& sect, const std::string& key) const
	{
		if (hasSection(sect))
			return this->sections_.at(sect).count(key) > 0 ? true : false;
		return false;
	}

	bool IniConfig::hasSection(const std::string& sect) const
	{
		return this->sections_.count(sect) > 0 ? true : false;
	}

	std::string IniConfig::origin() const
	{
	}

	const IniConfig::Section& IniConfig::operator[](std::string_view sect) const
	{
	}

	IniConfig::Section& IniConfig::operator[](std::string_view sect)
	{
	}
}