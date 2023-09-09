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
#include "nodeflow/typedefs.hpp"
#include <random>

namespace nf
{
class UUID
{
public:
    UUID();
    explicit UUID(std::uint64_t uuid);

    [[nodiscard]] static UUID create()
    {
        return UUID{};
    }
    [[nodiscard]] static UUID createFrom(std::uint64_t uuid)
    {
        return UUID{ uuid };
    }

    inline operator std::uint64_t() const
    {
        return m_uuid;
    }

private:
    std::uint64_t m_uuid;

private:
    static std::random_device s_seeder;
    static std::mt19937_64 s_engine;
    static std::uniform_int_distribution<std::uint64_t> s_dist;
};
} // namespace nf

namespace std
{
template <typename T>
struct hash;

template <>
struct hash<nf::UUID>
{
    std::size_t operator()(const nf::UUID& uuid) const
    {
        return (uint64_t)uuid;
    }
};
} // namespace std