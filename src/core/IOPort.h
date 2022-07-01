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


#include "3rdparty/entt/single_include/entt/entt.hpp"

#include "ValueWrapper.h"

namespace nf
{
	class PortView;

	template<typename T>
	class InputPort
	{
	public:
		using type_t = T;
		static constexpr auto typeID = entt::type_hash<T>::value();
		static constexpr auto streamable = has_ostream_operator_v<T>;

	private:
		int portIndex = -1;
	};





	template<typename T>
	class OutputPort : public ValueWrapper<T>
	{
		friend class PortView;
	public:
		static constexpr bool valid = true;
	public:
		OutputPort() = default;
		OutputPort(const T& defaultVal)
			: ValueWrapper<T>(defaultVal)
		{}

	private:
		int portIndex = -1;
	};

	template<>
	class OutputPort<void> : public ValueWrapper<void>
	{
		friend class PortView;
	public:
		static constexpr bool valid = false;
	public:
		OutputPort() = default;

	private:
		int portIndex = -1;
	};





	template<typename T, size_t minPorts = 1, size_t maxPorts = 255, unsigned int defaultPorts = minPorts>
	class InputPortGroup
	{
		static_assert(minPorts < maxPorts, "minPorts have to be smaller than maxPorts");
		static_assert(defaultPorts >= minPorts, "defaultPorts smaller than min limit");

	public:
		static constexpr auto typeID = entt::type_hash<T>::value();
		static constexpr auto minPorts = minPorts;
		static constexpr auto maxPorts = maxPorts;

		inline void setDefaultPortCount(unsigned int count)
		{
			portCount = count;
		}

		inline unsigned int portCount()
		{
			return portCount_;
		}

	private:
		unsigned int portCount_ = defaultPorts;
	};


	class PortView
	{
	public:
		template<typename T>
		PortView(const OutputPort<T>& port)
			: typeID(port.typeID), view(&port.value)
		{
		}

		template<typename T>
		const T* peek() const
		{
			static constexpr auto T_typeID = entt::type_hash<T>::value();
			if (typeID != T_typeID)
				return nullptr;
			return static_cast<const T*>(view);
		}

		const void* data() const
		{
			return view;
		}

		bool empty() const
		{
			return view == nullptr;
		}

		void reset()
		{
			typeID = 0;
			view = nullptr;
		}

	private:
		uint32_t typeID;
		const void* view = nullptr;
	};


}