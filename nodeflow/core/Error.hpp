/*
- nodeflow -
BSD 3-Clause License

Copyright (c) 2022, Ruwen Kohm
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#include <format>
#include <string>

#include "nodeflow/utility/Expected.hpp"

namespace nf {
	class Error {
	public:
		Error(int code)
			: m_code(code)
		{}

		Error(const std::string& msg)
			: m_message(msg)
		{}

		Error(const std::string& msg, int code)
			: m_message(msg)
			, m_code(code)
		{}

	public:
		[[nodiscard]] static Error fromCode(int code) { return Error(code); }

		[[nodiscard]] static Error fromMessage(const std::string& msg) { return Error(msg); }

		bool hasCode() const { return m_code != 0; }

		bool hasMessage() const { return !m_message.empty(); }

		int code() const { return m_code; }

		std::string message() const { return m_message; }

		bool operator ==(const Error& other) const
		{
			return ((m_code == other.m_code) && (m_message == other.m_message));
		}

	private:
		std::string m_message;
		int m_code = 0;
	};

	template<typename T>
	using ErrorOr = nf::Expected<T, Error>;
}

template<class CharT>
struct std::formatter<nf::Error, CharT> : std::formatter<CharT> {
	// parse() is inherited from the base class

	// Define format() by calling the base class implementation with the wrapped value
	template<class FormatContext>
	auto format(const nf::Error& error, FormatContext& fc) const {
		return std::format_to(fc.out(), "Error[{}]: {}", error.code(), error.message());
	}
};