#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <sstream>
#include <filesystem>
#include <regex>

namespace cpputils
{
	using Matches = std::vector<std::string>;
	std::regex wildcardToRegex(const std::string& wildcard, bool caseSensitive = true);
	bool wildmatch(const std::string& sample, const std::string& wildcard);
	Matches wildmatchVec(const std::vector<std::string>& samples, const std::regex& rgx);
	Matches wildmatchVec(const std::vector<std::string>& samples, const std::string& wildcard);

	// Directory Class is inspired by QDir from the Qt-Framework and provides access to directories and their contents
	class FileDirectory
	{
	public:
		enum FilterFlag { Dirs = 0, Files = 1, All = 2 };
		enum IterFlag { Recursive = 0, Shallow = 1 };

	public:
		FileDirectory(const std::string& dir_path);

		bool isDirectory() const;
		static bool isDirectory(const std::string& dir_path);
		static bool createDirectory(const std::string& dir_path);

		std::string path() const;
		std::string dirName() const;
		std::string findEntry(const std::string& fname) const;
		bool containsEntry(const std::string& fname) const;

		std::string absolutePath() const;
		static std::string absolutePath(const std::string& file_path);
		std::string relativeFilePath(const std::string& file_name) const;
		std::string absoluteFilePath(const std::string& file_name) const;

		Matches listEntries(IterFlag i = Shallow, FilterFlag f = All) const;
		Matches listEntries(const std::string& fileFilters, IterFlag i = Shallow, FilterFlag f = All) const;

	private:
		template<typename DirIterator>
		Matches listEntriesImpl(FilterFlag f) const
		{
			Matches file_entries;
			for (const auto& entry : DirIterator(absolutePath()))
			{
				if (f == FilterFlag::Files)
				{
					if (!std::filesystem::is_directory(entry))
					{
						file_entries.push_back(entry.path().filename().string());
						continue;
					}
				}
				else if (f == FilterFlag::Dirs)
				{
					if (std::filesystem::is_directory(entry))
					{
						file_entries.push_back(entry.path().filename().string());
						continue;
					}
				}
				else
				{
					file_entries.push_back(entry.path().filename().string());
					continue;
				}
			}
			return file_entries;
		}

	private:
		std::string dirpath_;
	};
}
