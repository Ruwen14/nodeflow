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

#include "../core/type_tricks.hpp"
#include "type_reflection.hpp"

namespace nf
{
template <auto Func>
struct reflfunc
{
    using signature_t = typename nf::FuncSignature<decltype(std::function{ Func })>;
    using return_t = signature_t::ReturnType_t;
    using argument_ts = signature_t::ParamTypes_t;

    static constexpr bool ismethod() noexcept
    {
        return std::is_member_function_pointer_v<decltype(Func)>();
    }

    static constexpr typeid_t id() noexcept
    {
        return 123;
    }

    static constexpr auto name() noexcept
    {
        return "";
    }

    static constexpr auto returnTypeName() noexcept
    {
        return nf::type_name<return_t>();
    }

    static constexpr auto argTypeNames() noexcept
    {
        return nf::refltypes<argument_ts>::names();
    }

    static constexpr auto argCount() noexcept
    {
        return std::tuple_size_v<argument_ts>;
    }
};
} // namespace nf