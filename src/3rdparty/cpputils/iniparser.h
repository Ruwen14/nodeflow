#pragma once

#include "stringutils.h"
#include "typecheck.h"

#include <string>
#include <string_view>
#include <map>
#include <set>


namespace cpputils
{
	struct IniParams;

	class IniConfig;

	// use stringstream instead with << >> operators
	IniConfig iniRead(std::string_view fpath);
	void iniWrite(std::string_view fpath, const IniConfig& opt);

	class IniConfig
	{
	public:
		using Section = std::map<std::string, std::string>;
		using Sections = std::map<std::string, Section>;
		enum ArrayDelimFlag { ARRAY_DELIM_SPACE, ARRAY_DELIM_COMA };

		IniConfig() = default;

		std::set<std::string> keys(const std::string& sect) const;
		std::set<std::string> sections() const;

		bool hasKey(const std::string& sect, const std::string& key) const;
		bool hasSection(const std::string& sect) const;

		void clear();


		bool empty() const noexcept { return this->sections_.empty(); }
		bool contains(const std::string& key /*recursive*/) const;



		explicit operator bool() const noexcept { return this->empty(); }

		std::string origin() const;

		// Think about if you relaly want it cause at is better
		const Section& operator[](std::string_view sect) const;
		Section& operator[](std::string_view sect);

		template<typename T = std::string>
		T getValue(std::string_view sect, std::string_view key) const
		{
			static_assert(!is_std_container<T>::value, "For parsing Array-like structures, consider 'getCollection'");
			//if constexpr (std::is_same<T, std::string>()) {
			//	return value;
		}

		template<typename T = std::string>
		T getValueOr(std::string_view sect, std::string_view key, const T& dlft) const
		{
			static_assert(is_std_container<T>::value, "For parsing Array-like structures, consider 'getCollection'");
		}

		template<typename T>
		T getVector(const std::string& sect, const std::string& key) const
		{
			static_assert(is_std_container<T>::value, "Type 'T' is not a container.");
			//this->sections_.at(sect).at(key);

			//if constexpr (std::is_same<T, std::string>()) {
			//	return value;

			auto sectiter = this->sections_.find(sect);
			if (sectiter != this->sections_.end())
			{
				auto keyiter = sectiter->second.find(key);
				if (keyiter != sectiter->second.end())
				{
					auto result = keyiter->second;
				}
				// Throw Invalid Key
			}
			// Throw Invalid Section
		}

		template<typename T>
		T getVector(const std::string& sect, const std::string& key, ArrayDelimFlag flag) const
		{
			static_assert(is_std_container<T>::value, "Type 'T' is not a container.");
		}

		template<typename T>
		T getVectorOr(std::string_view sect, std::string_view key, const T& dlft) const
		{
			static_assert(is_std_container<T>::value, "Type 'T' is not a container.");
		}

		template<typename T>
		T getArray(const std::string& sect, const std::string& k, ArrayDelimFlag flag) const
		{

		}


		template<typename T>
		void set(std::string_view sect, std::string_view key, const T& val)
		{
			if constexpr (std::is_same_v<T, std::string>)
			{
				//blabla
			}
			auto val_str = stringify(val);
		}








		template<typename Iter>
		void set(std::string_view sect, std::string_view key, Iter begin, Iter end)
		{
			setCollection(sect, key, begin, end, ARRAY_DELIM_COMA);
		}

		template<typename Iter>
		void setCollection(std::string_view sect, std::string_view key,
			Iter begin, Iter end, ArrayDelimFlag flag)
		{
		}

	protected:

		Sections sections_;
		std::string origin_ = ";";
	};
}