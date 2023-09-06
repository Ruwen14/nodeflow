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
#include "nodeflow/reflection/type_reflection.hpp"

namespace nf::detail
{
	class DataHandle
	{
	public:
		DataHandle() = default;

		template<typename T>
		DataHandle(T& data, typeid_t typeID)
			: m_typeid(typeID), m_dataptr(static_cast<void*>(&data))
		{
		}

		template<typename Type>
		Type* getMutable()
		{
			static constexpr typeid_t targetID = nf::type_id<Type>();
			if (m_typeid != targetID)
			{
				return nullptr;
			}
			return static_cast<Type*>(m_dataptr);
		}

		template<typename Type>
		const Type* get() const
		{
			static constexpr typeid_t targetID = nf::type_id<Type>();
			if (m_typeid != targetID)
			{
				return nullptr;
			}
			return static_cast<const Type*>(m_dataptr);
		}

		template<typename T> void assign(T& data, typeid_t typeID)
		{
			m_typeid = typeID;
			m_dataptr = static_cast<void*>(&data);
		}

		const void* data() const noexcept
		{
			return m_dataptr;
		}

		inline bool valid() const noexcept
		{
			return m_dataptr != nullptr && m_typeid != 0;
		}

		operator bool() const noexcept
		{
			return valid();
		}

		void reset()
		{
			m_typeid = 0;
			m_dataptr = nullptr;
		}

		template<typename Type>
		bool isType() const
		{
			return (valid() && m_typeid == nf::type_id<Type>());
		}

		inline typeid_t typeID() const noexcept
		{
			return m_typeid;
		}

	public:
		typeid_t m_typeid = 0;
		void* m_dataptr = nullptr;
	};
}