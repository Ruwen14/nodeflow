#pragma once

#pragma once
#include <iostream>
#include <vector>

namespace cpputils
{
	template <class T> constexpr std::string_view type_name()
	{
		using namespace std;
#ifdef __clang__
		string_view p = __PRETTY_FUNCTION__;
		return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
		string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
		return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
		return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
		string_view p = __FUNCSIG__;
		return string_view(p.data() + 94, p.size() - 94 - 7);
#endif
	}

	constexpr const char* FILENAME_MACRO(const char* path) {
		const char* file = path;
		while (*path) {
			if (*path++ == '/' || *path++ == '\\') {
				file = path;
			}
		}
		return file;
	}

	template <typename T>
	void cDebugInfoBegin(T t, char const* file, int line)
	{
		std::cout
			<< "<debug>(" << __TIME__ << "]"
			<< "[" << FILENAME_MACRO(file) << ":" << line << "][" << type_name<decltype(t)>() << "]--> ";
	}

	template <typename T>
	void cDebug(T t, char const* file, int line)
	{
		cDebugInfoBegin(t, file, line);
		std::cout << t << "\n";
	}

	template <typename T>
	void cDebug(const std::vector<T> vec, char const* file, int line)
	{
		cDebugInfoBegin(vec, file, line);
		std::cout << vec << "[S: " << vec.size() << "]" << "[C: " << vec.capacity() << "]\n";
	}

#define cDebug(msg) cDebug(msg, __FILE__, __LINE__)
}