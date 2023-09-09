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
#include <functional>
#include <type_traits>

class TypeInfo
{
};
class NodeSignal
{
};

namespace detail
{
template <typename, typename, typename = void>
struct is_func_signature : std::false_type
{
};

template <typename TFunc, typename Ret, typename... Args>
struct is_func_signature<
    TFunc,
    Ret(Args...),
    typename std::enable_if<std::is_convertible<TFunc, std::function<Ret(Args...)>>::value>::type>
    : public std::true_type
{
};

class TypeConversionBase
{
public:
    TypeConversionBase(TypeInfo from, TypeInfo to)
        : from_(std::move(from))
        , to_(std::move(to))
    {
    }

    virtual ~TypeConversionBase() = default;

    const TypeInfo& to() const noexcept
    {
        return to_;
    }
    const TypeInfo& from() const noexcept
    {
        return from_;
    }

    virtual bool convert(const NodeSignal& from, NodeSignal& out) const = 0;

private:
    TypeInfo from_;
    TypeInfo to_;
};

template <typename FromType, typename ToType, typename Callable>
class TypeConversionImpl : public TypeConversionBase
{
public:
    TypeConversionImpl(TypeInfo from, TypeInfo to)
        : TypeConversionBase(from, to)
    {
    }

    bool convert(const NodeSignal& from, NodeSignal& out) const override
    {
        // 				FromType from = sig.getValue<FromType>();
        // 				ToType to = this->fun_(from);
        // 				NodeSignal out;
        // 				out.setValue<ToType>(to);
        // 				return out
    }

private:
    Callable fun_;
};
} // namespace detail