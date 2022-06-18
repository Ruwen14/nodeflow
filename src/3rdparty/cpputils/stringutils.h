#pragma once
#include "typecheck.h"
#include "magic_enum.h"

#include <string>
#include <vector>
#include <charconv>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <optional>
#include <iostream>
#include <type_traits>
#include <array>
#include <complex>

namespace cpputils
{
	inline void str_lowercase(std::string& sample)
	{
		std::transform(sample.begin(), sample.end(), sample.begin(), [](char c) {return static_cast<char>(std::tolower(c)); });
	}

	inline void str_lowercase(const std::string& src, std::string& dst)
	{
		dst = src;
		str_lowercase(dst);
	}

	inline void str_uppercase(std::string& sample)
	{
		std::transform(sample.begin(), sample.end(), sample.begin(), [](char c) {return static_cast<char>(std::toupper(c)); });
	}

	inline void str_uppercase(const std::string& src, std::string& dst)
	{
		dst = src;
		str_uppercase(dst);
	}

	inline void str_remove_ws(std::string& sample)
	{
		sample.erase(std::remove_if(sample.begin(), sample.end(), ::isspace), sample.end());
	}

	inline std::vector<std::string_view> str_split(std::string_view strv, const char delim)
	{
		std::vector<std::string_view> output;
		const auto target_size = std::count(strv.begin(), strv.end(), delim);
		if (target_size == 0)
			return {};
		output.reserve(target_size + 1);
		size_t first = 0;
		while (first < strv.size())
		{
			const auto second = strv.find_first_of(delim, first);
			if (first != second)
			{
				output.emplace_back(strv.substr(first, second - first));
			}
			if (second == std::string_view::npos)
				break;

			first = second + 1;
		}
		return output;
	}

	template <typename Iter>
	std::string str_join(Iter begin, Iter end, const std::string_view& delim);

	template<typename T>
	inline std::string stringify(const T& t);

	template<typename T>
	using cast_result = std::optional<T>;
	namespace converters
	{
		template <typename T, typename Enable = void>
		struct FromStrConverter {
			static inline cast_result<T> convert(std::string_view str) {
				static_assert(sizeof(T) == 0, "No specialized converter for given type found.");
			}
		};

		template <typename T>
		struct FromStrConverter<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
			static inline cast_result<T> convert(std::string_view str) {
				T result;
				auto error = std::from_chars(str.data(), str.data() + str.size(), result);
				if (error.ec != std::errc{})
				{
					return {};
				}
				return result;
			}
		};

		template <>
		struct FromStrConverter<std::string> {
			static inline cast_result<std::string> convert(std::string_view str) {
				return { std::string{str} };
			}
		};

		template <>
		struct FromStrConverter<bool> {
			static inline cast_result<bool> convert(std::string_view str)
			{
				std::string strD{ str };
				str_lowercase(strD);
				static const std::array<std::pair<std::string_view, bool>, 8> s2b{ {
					{"1", true}, { "true", true }, { "yes", true }, { "on", true },
					{ "0", false }, { "false", false }, { "no", false }, { "off", false }} };
				for (const auto& p : s2b)
				{
					if (p.first == strD)
						return p.second;
				}
				return {};
			}
		};

		template <typename EnumType>
		struct FromStrConverter<EnumType, typename std::enable_if<std::is_enum_v<EnumType>>::type> {
			static inline cast_result<EnumType> convert(std::string_view enum_str) {
				auto pos = enum_str.find("::");
				if (pos != std::string_view::npos)
					enum_str = enum_str.substr(pos + 2);
				auto enum_type = magic_enum::enum_cast<EnumType>(enum_str);
				return enum_type;
			}
		};

		template <typename T, typename Enable = void>
		struct ToStrConverter {
			static inline std::string convert(const T& t) {
				static_assert(sizeof(T) == 0, "No specialized converter for given type found.");
			}
		};

		template <>
		struct ToStrConverter<std::string> {
			static inline std::string convert(const std::string& t) {
				return t;
			}
		};

		template <>
		struct ToStrConverter<const char*> {
			static inline std::string convert(const char* t) {
				return std::string(t);
			}
		};

		template <>
		struct ToStrConverter<std::string_view> {
			static inline std::string convert(const std::string_view t) {
				return std::string(t);
			}
		};

		template <typename T>
		struct ToStrConverter<T,
			typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			static inline std::string convert(const T t) {
				return std::to_string(t);
			}
		};

		template <>
		struct ToStrConverter<bool> {
			static inline std::string convert(const bool t) {
				return t ? "true" : "false";
			}
		};

		template <typename T>
		struct ToStrConverter<std::optional<T>> {
			static inline std::string convert(const std::optional<T> opt) {
				if (opt)
				{
					return stringify(opt.value());
				}
				return "nullopt";
			}
		};

		template <typename T>
		struct ToStrConverter<std::complex<T>> {
			static inline std::string convert(const std::complex<T>& cmplx) {
				std::ostringstream out;
				out << cmplx.real() << '+' << cmplx.imag() << "i";
				return out.str();
			}
		};

		template <typename first, typename second>
		struct ToStrConverter<std::pair<first, second>> {
			static inline std::string convert(const std::pair<first, second>& p) {
				std::string out;
				out += '(';
				out += stringify(p.first);
				out += ", ";
				out += stringify(p.second);
				out += ')';
				return out;
			}
		};

		template <typename EnumType>
		struct ToStrConverter<EnumType,
			typename std::enable_if<std::is_enum_v<EnumType>>::type> {
			static inline std::string convert(const EnumType enum_value) {
				std::string out;
				auto enum_value_str = magic_enum::enum_name(enum_value);
				auto enum_type_str = magic_enum::enum_type_name<EnumType>();
				out += enum_type_str;
				out += "::";
				out += enum_value_str;
				return out;
			}
		};

		template <typename T>
		struct ToStrConverter<T,
			typename std::enable_if<is_std_container<T>::value>::type> {
			static inline std::string convert(const T& t) {
				return "[" + str_join(t.begin(), t.end(), ", ") + "]";
			}
		};

		template <typename T>
		struct ToStrConverter<T*> {
			static inline std::string convert(T* ptr) {
				if (ptr)
				{
					return typeid(T*).name();
				}
				return "nullptr";
			}
		};


	} // END namespace converters

	template<typename T>
	inline cast_result<T> str_cast(std::string_view str)
	{
		return converters::FromStrConverter<T>::convert(str);
	}

	template<typename T>
	inline std::string stringify(const T& t)
	{
		return converters::ToStrConverter<T>::convert(t);
	}

	template<typename T>
	cast_result<std::vector<T>> str_cast_vec(std::string sample, const char delim,
		const char prefix, const char suffix)
	{
		str_remove_ws(sample);
		std::string_view view{ sample };
		size_t prepos = view.find_first_of(prefix) + 1;
		size_t sufpos = view.find_last_of(suffix) - 1;
		const auto contents_view = view.substr(prepos, sufpos);
		const auto split = str_split(view, delim);

		if (split.empty())
			return {};

		std::vector<T> out;
		out.reserve(split.size());
		for (const auto v : split)
		{
			if (auto sucess = str_cast<T>(v))
				out.push_back(*sucess);
		}
		return out;
	}

	template <typename Iter>
	inline std::string str_join(Iter begin, Iter end, const std::string_view& delim)
	{
		std::string result;
		for (Iter it = begin; it != end; it++)
		{
			if (!result.empty())
				result += delim;
			result += stringify(*it);
		}
		return result;
	}

	//template<typename T>

	//template<typename Numeric>
	//std::vector<Numeric> str_to_numeric_vec(std::string sample, const char delim, const char prefix, const char suffix)
	//{
	//	str_remove_ws(sample);
	//	std::string_view sview{ sample };
	//	size_t prepos = sview.find(prefix) + 1;
	//	size_t sufpos = sview.find_last_of(suffix) - 1;
	//	auto contents_view = sview.substr(prepos, sufpos);

	//	std::vector<Numeric> out;
	//	out.reserve(std::count(contents_view.begin(), contents_view.end(), delim) + 1);

	//	size_t first = 0;
	//	while (first < contents_view.size())
	//	{
	//		const auto second = contents_view.find_first_of(delim, first);
	//		if (first != second)
	//		{
	//			Numeric elem;
	//			if (str_to_numeric<Numeric>(contents_view.substr(first, second - first), elem))
	//			{
	//				out.push_back(elem);
	//			}
	//		}
	//		if (second == std::string_view::npos)
	//			break;

	//		first = second + 1;
	//	}

	//	return out;
	//}
}
