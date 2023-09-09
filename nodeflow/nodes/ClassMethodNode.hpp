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

#include "nodes/FunctorNode.hpp"

template <auto method>
class ClassMethodNode : public FlowNode
{
public:
    using MSig_t = nf::FuncSignature<std::function<decltype(method)>>;
    using MClass_t = MSig_t::ClassType_t;
    using MReturn_T = MSig_t::ReturnType_t;
    using MArgument_ts = MSig_t::ParamTypes_t;

public:
    InputPort<MClass_t> m_thisPort;
};

//
//
// template<auto method, class clazz>
// class TestFunctor
// {
// public:
//
//
// 	TestFunctor(const TestClass& clazz)
// 		: c(clazz)
// 	{
//
// 	}
//
// 	void process()
// 	{
// 		auto e = std::invoke(method, c);
// 		pprint(e);
// 	}
// 	clazz c;
// };

// template<class class_>
// class ClassNodeBuilder
// {
// public:
// 	using classType = class_;
//
//
// 	template<auto member>
// 	auto& property(std::string_view name)
// 	{
// 		using memberType = nf::deduce_member_type<decltype(member)>::type;
// 		return *this;
// 	}
//
// 	template<auto func>
// 	auto& method(std::string_view name)
// 	{
// 		return *this;
// 	}
//
// };