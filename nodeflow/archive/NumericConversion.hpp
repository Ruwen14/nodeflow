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
#include "TypeTricks.h"


namespace nf
{
	enum class NumericConversionState
	{
		Valid_Conversion,
		Bad_Positive_Overflow,
		Bad_Negative_Overflow,
		Bad_Precision_Lost,
		Bad_Conversion
	};

	enum class FundamentalFlag : uint8_t
	{
		NonFundamental,
		Void,
		Nullptr_t,
		Bool,
		Char,
		SignedChar,
		UnsignedChar,
		ShortInt,
		UnsignedShortInt,
		Int,
		UnsignedInt,
		LongInt,
		UnsignedLongInt,
		LongLongInt,
		UnsignedLongLongInt,
		Float,
		Double,
		LongDouble
	};
	
	template<typename Type>
	consteval FundamentalFlag fundamentalFlag()
	{
		if constexpr (!std::is_fundamental_v<Type>)
			return FundamentalFlag::NonFundamental;

		using CommonBase = std::common_type_t<Type>;

		return
			(std::is_void_v<CommonBase>) ? (FundamentalFlag::Void)
			: (std::is_null_pointer_v<CommonBase>) ? (FundamentalFlag::Nullptr_t)
			: (std::is_same_v<CommonBase, bool>) ? (FundamentalFlag::Bool)
			: (std::is_same_v<CommonBase, char>) ? (FundamentalFlag::Char)
			: (std::is_same_v<CommonBase, signed char>) ? (FundamentalFlag::SignedChar)
			: (std::is_same_v<CommonBase, unsigned char>) ? (FundamentalFlag::UnsignedChar)
			: (std::is_same_v<CommonBase, short int>) ? (FundamentalFlag::ShortInt)
			: (std::is_same_v<CommonBase, unsigned short int>) ? (FundamentalFlag::UnsignedShortInt)
			: (std::is_same_v<CommonBase, int>) ? (FundamentalFlag::Int)
			: (std::is_same_v<CommonBase, unsigned int>) ? (FundamentalFlag::UnsignedInt)
			: (std::is_same_v<CommonBase, long int>) ? (FundamentalFlag::LongInt)
			: (std::is_same_v<CommonBase, unsigned long int>) ? (FundamentalFlag::UnsignedLongInt)
			: (std::is_same_v<CommonBase, long long int>) ? (FundamentalFlag::LongLongInt)
			: (std::is_same_v<CommonBase, unsigned long long int>) ? (FundamentalFlag::UnsignedLongLongInt)
			: (std::is_same_v<CommonBase, float>) ? (FundamentalFlag::Float)
			: (std::is_same_v<CommonBase, double>) ? (FundamentalFlag::Double)
			: (std::is_same_v<CommonBase, long double>) ? (FundamentalFlag::LongDouble)
			: (FundamentalFlag::NonFundamental);
	}

	inline bool isArithmetic(FundamentalFlag flag)
	{
		return !(flag == FundamentalFlag::NonFundamental || flag == FundamentalFlag::Void || flag == FundamentalFlag::Nullptr_t);
	}

	inline bool isVoid(FundamentalFlag flag)
	{
		return flag == FundamentalFlag::Void;
	}

	inline bool isValidType(FundamentalFlag  flag)
	{
		return !(flag == FundamentalFlag::Void || flag == FundamentalFlag::Nullptr_t);
	}


	template<typename ToType, typename FromType>
	NumericConversionState nf_numeric_cast()
	{
	}
	



	
}
