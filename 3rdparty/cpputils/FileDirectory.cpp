#include "FileDirectory.h"
#include "cpputils.h"

namespace cpputils
{
	std::regex wildcardToRegex(const std::string& wildcard, bool caseSensitive /*= true*/)
	{
		//regexString = std::regex_replace(regexString, std::regex("\\\\"), "\\\\");
		//	regexString = std::regex_replace(regexString, std::regex("\\^"), "\\^");
		//regexString = std::regex_replace(regexString, std::regex("\\."), "\\.");
		//regexString = std::regex_replace(regexString, std::regex("\\$"), "\\$");
		//auto regexString = std::regex_replace(wildcard, std::regex("\\|"), "\\|");
		//regexString = std::regex_replace(regexString, std::regex("\\("), "\\(");
		//regexString = std::regex_replace(regexString, std::regex("\\)"), "\\)");
		//regexString = std::regex_replace(regexString, std::regex("\\{"), "\\{");
		//regexString = std::regex_replace(regexString, std::regex("\\{"), "\\}");
		//regexString = std::regex_replace(regexString, std::regex("\\["), "\\[");
		//regexString = std::regex_replace(regexString, std::regex("\\]"), "\\]");
		//regexString = std::regex_replace(regexString, std::regex("\\+"), "\\+");
		//regexString = std::regex_replace(regexString, std::regex("\\/"), "\\/");
		// Convert wildcard specific chars '*?' to their regex equivalents:
		//regexString = std::regex_replace(regexString, std::regex("\\?"), ".");

		auto regexString = std::regex_replace(wildcard, std::regex("\\*"), ".*");
		return std::regex(regexString, caseSensitive ? std::regex_constants::ECMAScript : std::regex_constants::icase);
	}

	bool wildmatch(const std::string& sample, const std::string& wildcard)
	{
		auto rgx = wildcardToRegex(wildcard);
		return std::regex_match(sample, rgx);
	}

	Matches wildmatchVec(const std::vector<std::string>& samples, const std::regex& rgx)
	{
		std::vector<std::string> matches;
		std::copy_if(std::begin(samples), std::end(samples),
			std::back_inserter(matches),
			[&](std::string const& name) {
				return std::regex_match(name, rgx);
			}
		);
		return matches;
	}

	Matches wildmatchVec(const std::vector<std::string>& samples, const std::string& wildcard)
	{
		return wildmatchVec(samples, wildcardToRegex(wildcard));
	}

	FileDirectory::FileDirectory(const std::string& dir_path)
		: dirpath_(dir_path)
	{
	}

	bool FileDirectory::isDirectory() const
	{
		return std::filesystem::is_directory(this->dirpath_);
	}

	bool FileDirectory::isDirectory(const std::string& dir_path)
	{
		return std::filesystem::is_directory(dir_path);
	}

	std::string FileDirectory::path() const
	{
		return this->dirpath_;
	}

	bool FileDirectory::createDirectory(const std::string& dir_path)
	{
		std::filesystem::create_directory(dir_path);
	}

	std::string FileDirectory::absolutePath() const
	{
		return std::filesystem::absolute(this->dirpath_).string();
	}

	std::string FileDirectory::absolutePath(const std::string& file_path)
	{
		return std::filesystem::absolute(file_path).string();
	}

	std::string FileDirectory::dirName() const
	{
		return this->dirpath_.substr(this->dirpath_.find_last_of("/\\") + 1);
	}

	std::string FileDirectory::findEntry(const std::string& fname) const
	{
		std::filesystem::path target(fname);
		for (const auto& entry : std::filesystem::recursive_directory_iterator(this->dirpath_))
		{
			if (entry.path().filename() == target)
			{
				return entry.path().string();
			}
		}
		return "";
	}

	std::string FileDirectory::relativeFilePath(const std::string& file_name) const
	{
		const char last_element = this->dirpath_.back();
		if (last_element != '/' && last_element != '\\')
		{
			return this->dirpath_ + '/' + file_name;
		}
		return this->dirpath_ + file_name;
	}

	std::string FileDirectory::absoluteFilePath(const std::string& file_name) const
	{
		std::string rel_path = relativeFilePath(file_name);
		return std::filesystem::absolute(std::filesystem::path(rel_path)).string();
	}

	Matches FileDirectory::listEntries(IterFlag i /*= Shallow*/, FilterFlag f /*= All*/) const
	{
		if (i == IterFlag::Shallow)
		{
			return listEntriesImpl<std::filesystem::directory_iterator>(f);
		}
		if (i == IterFlag::Recursive)
		{
			return listEntriesImpl<std::filesystem::recursive_directory_iterator>(f);
		}
	}

	Matches FileDirectory::listEntries(const std::string& fileFilters, IterFlag i /*= Shallow*/, FilterFlag f /*= All*/) const
	{
		return wildmatchVec(listEntries(i, f), fileFilters);
	}

	bool FileDirectory::containsEntry(const std::string& fname) const
	{
		std::filesystem::path target(fname);
		for (const auto& entry : std::filesystem::recursive_directory_iterator(this->dirpath_))
		{
			if (entry.path().filename() == target)
			{
				return true;
			}
		}
		return false;
	}
}