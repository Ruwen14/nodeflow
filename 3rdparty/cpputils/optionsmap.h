#pragma once
#include <string>
#include <variant>
#include <map>
#include <vector>
#include <sstream>

namespace cpputils
{
	template<class ValueType>
	class OptionsMap
	{
	public:
		OptionsMap() = default;
		OptionsMap(const std::map<std::string, ValueType>& opt)
			: optmap_(opt) {}

		void store(const std::string& key, ValueType val)
		{
			this->optmap_[key] = val;
		}

		void update(const OptionsMap& opt)
		{
			this->optmap_ = opt.optmap_;
		}

		ValueType get(const std::string& key) const
		{
			return this->optmap_.at(key);
		}

		ValueType get_default(const std::string& key, ValueType dflt) const
		{
			auto iter = this->optmap_.find(key);
			if (iter != this->optmap_.end())
			{
				return iter->second;
			}
			return dflt;
		}

		std::vector<std::string> get_keys() const
		{
			std::vector<std::string> keyvec;
			for (auto const& opt : this->optmap_)
			{
				keyvec.push_back(opt.first);
			}
			return keyvec;
		}

		bool has_key(const std::string& key) const
		{
			if (this->optmap_.find(key) == this->optmap_.end())
			{
				return false;
			}
			return true;
		}

		std::string dump() const
		{
			std::stringstream ss;
			for (const auto& [k, v] : this->optmap_)
			{
				ss << "[" << k << "] : " << v << "\n";
			}
			return ss.str();
		}

	private:
		std::map<std::string, ValueType> optmap_;
	};

	template<class... ValueTypes>
	class VariantOptionsMap
	{
	public:

		VariantOptionsMap() = default;
		VariantOptionsMap(const std::map<std::string, std::variant<ValueTypes...>>& opt)
			: varoptmap_(opt) {}

		void store(const std::string& key, const std::variant<ValueTypes...>& val)
		{
			this->varoptmap_[key] = val;
		}

		void update(const VariantOptionsMap& opt)
		{
			this->varoptmap_ = opt.varoptmap_;
		}

		template<typename T>
		T get(const std::string& key) const
		{
			return std::get<T>(this->varoptmap_.at(key));
		}

		template<typename T>
		T get_default(const std::string& key, T dflt) const
		{
			if (has_key(key))
			{
				return std::get<T>(this->varoptmap_.at(key));
			}
			return dflt;
		}

		std::vector<std::string> get_keys() const
		{
			std::vector<std::string> keyvec;
			for (auto const& opt : this->varoptmap_)
			{
				keyvec.push_back(opt.first);
			}
			return keyvec;
		}

		bool has_key(const std::string& key) const
		{
			if (this->varoptmap_.find(key) == this->varoptmap_.end())
			{
				return false;
			}
			return true;
		}

		std::string dump() const
		{
			std::stringstream ss;
			for (const auto& [k, v] : this->varoptmap_)
			{
				ss << "[" << k << "] : ";
				std::visit([&ss](const auto& x) { ss << x; }, v);
				ss << "\n";
			}
			return ss.str();
		}

		int type_idx(const std::string& key) const
		{
			return this->varoptmap_.at(key).index();
		}

	private:
		std::map<std::string, std::variant<ValueTypes...>> varoptmap_;
	};
}