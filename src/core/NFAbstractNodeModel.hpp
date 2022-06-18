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
// #include "core/NFTypeInfo.h"

#include <string>
#include <vector>

namespace nodeflow
{
	enum class PinFlag
	{
		In,
		Out
	};

	class NFAbstractNodeModel
	{
		/* Store Pin struct instead? Acess spead?
		struct Pin
		{
			std::string name = "";
			NFTypeInfo ti;
			PinPolicy pp;
		}; */



	public:
// 		using PinType = NFTypeInfo;
		using PinType = int;


		explicit NFAbstractNodeModel(
			const std::string& caption,
			const std::vector<PinType>& t_in,
			const std::vector<PinType>& t_out,
			const std::vector<std::string>& in_names,
			const std::vector<std::string>& out_names);

		explicit NFAbstractNodeModel(
			const std::string& caption,
			const std::vector<PinType>& t_in,
			const std::vector<PinType>& t_out);

		explicit NFAbstractNodeModel(
			const std::string& caption);

		NFAbstractNodeModel() = default;

		virtual ~NFAbstractNodeModel() = default;

	public:

		std::string name() const noexcept;

		void setName(const std::string& name) noexcept;

		std::string pinName(int pin, PinFlag f) const;

		void setPinName(int pin, PinFlag f, const std::string& name) noexcept;

		int pinCount(PinFlag f) const noexcept;

		const PinType& pinType(int pin, PinFlag f) const;

		/* INteraface for PinPolicy with {DisplayName, InputToMany or One}
		PinPolicy pinPolicy(int pint, PinFlag f) const noexcept;
		*/

	public:
		virtual bool hasInputPins() const noexcept;

		virtual bool hasOutputPins() const noexcept;

		virtual bool hasDynamicPorts(PinFlag f) const { return false; }

	public:
		// Calls propagate
		bool propagateData();

		/* Maybe for event stuff
		virtual std::unique_ptr<NFEvent> event() const = 0; */

	protected:
		virtual void propagate() = 0;



		/* Possible Interface for getting the input bus
		const NFNodeSignals& inputBus() const; */

		/* Possible Interface for getting the Outpus bus and setting it
		NFNodeSignals& outputBus() const; */

	protected:
		/* Possible Interface for dynamic Node
		bool addPort(const detail::NFTypeInfo& type, const std::string); */

		/* Possible Interface for dynamic Node
		bool removePort(const detail::NFTypeInfo& type, const std::string); */

		/* Possible Interface for dynamic Node Dat
		bool setPinData(int pin, PinFlag f, const PinType& t_data,
							const std::string& name = ""); */

							/* Setup Stuff done in constructor ?
							template<int PinCount, class... PinTypes>
							void setupInputPins(std::array<std::string, PinCount> pinnames) noexcept
							{
								static_assert(PinCount == sizeof...(PinTypes), "'PinCount' and 'PinTypes' of unequal length");
							}

							template<int PinCount, class... PinTypes>
							void setupOutputPins(std::array<std::string, PinCount> pinnames) noexcept
							{
								static_assert(PinCount == sizeof...(PinTypes), "'PinCount' and 'PinTypes' of unequal length");
							} */

	private:
		std::string name_ = "Nodeflow-Node";
		std::vector<PinType> inTypes_;
		std::vector<PinType> outTypes_;
		int inPins_ = 0;
		int outPins_ = 0;
	};
}
