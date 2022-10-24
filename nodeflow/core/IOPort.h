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


#include "entt/single_include/entt/entt.hpp"
#include "ValueWrapper.hpp"

#include <optional>
#include <functional>

namespace nf
{
	class Node;
	struct ExecutionPinOut
	{
		// ToDo: Maybe name = "branch1 | branch2" [if branchable]

		enum BranchFlag : std::uint8_t {
			Unbranchable = 0,
			FirstBranch = 1,
			SecondBranch = 2,
			StopExecution = 3
		};

		inline void MakeBranchable() noexcept
		{
			flag = FirstBranch;
		}

		inline bool Branchable() const noexcept
		{
			return flag != Unbranchable;
		}
		inline void BranchFirst() noexcept
		{
			if (flag != Unbranchable)
				flag = FirstBranch;
		}
		inline void BranchSecond() noexcept
		{
			if (flag != Unbranchable)
				flag = SecondBranch;
		}
		inline void ToggleBranch() noexcept
		{
			if (flag == FirstBranch)
				flag = SecondBranch;
			else if (flag == SecondBranch)
				flag = FirstBranch;
			else
				return;
		}

		inline void ForceStopExecution() noexcept
		{
			flag = StopExecution;
		}

		inline Node* GetLinkedNodeToBeExecuted() const noexcept
		{
			if (flag == StopExecution)
				return nullptr;

			if (flag == SecondBranch)
				return linkedNodeSecondBranch;
			return linkedNodeFirstBranch; // Case for Unbranchable and FirstBranch
		}

		inline bool IsConnected() const noexcept
		{
			return linkedNodeFirstBranch || linkedNodeSecondBranch;
		}

	private:
		Node* linkedNodeFirstBranch = nullptr;
		Node* linkedNodeSecondBranch = nullptr;
	public:
		BranchFlag flag = Unbranchable; // Either 0 or 1
	};








	class OutputPinHandle;

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
		friend class OutputPinHandle;
	public:
		static constexpr bool valid = true;
	public:
		OutputPort() = default;
		OutputPort(const T& defaultVal)
			: ValueWrapper<T>(defaultVal)
		{}

		// Serialization operator <<

	public:
		int portIndex = -1;
	};

	template<>
	class OutputPort<void> : public ValueWrapper<void>
	{
		friend class OutputPinHandle;
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

	// IDK maybe call id NfOutDataHandle like MDataHandle
	class OutputPinHandle
	{
	public:
		template<typename T>
		OutputPinHandle(OutputPort<T>& port)
			: typeID_(port.typeID), view(&port.value)
		{
		}

		template<typename Type>
		const Type* valuePtr() const noexcept
		{
			return valueRef<Type>();
		}

		template<typename Type>
		Type* valueRef() const noexcept
		{
			static constexpr auto Type_typeID = entt::type_hash<Type>::value();
			if (typeID_ != Type_typeID)
				return nullptr;
			return static_cast<Type*>(view);
		}

		const void* data() const noexcept
		{
			return view;
		}

		bool empty() const noexcept
		{
			return view == nullptr;
		}

		void reset()
		{
			typeID_ = entt::type_hash<void>::value();
			view = nullptr;
		}

		template<typename Type>
		bool isType()
		{
			static constexpr auto Type_typeID = entt::type_hash<Type>::value();
			return typeID_ == Type_typeID;
		}


		inline uint64_t typeID() const noexcept
		{
			return typeID_;
		}

	private:
		uint64_t typeID_ = entt::type_hash<void>::value();
		void* view = nullptr;
	};


}