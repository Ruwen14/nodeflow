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
#include <cstdint>
#include <iostream>
#include <optional>

#include "core/FlowEvent.hpp"
#include "core/UUID.hpp"
#include "typedefs.hpp"

#define NF_REGISTER_Type(ObjType)                              \
public:                                                        \
    static constexpr auto staticType = nf::type_id<ObjType>(); \
    nf::typeid_t type() const override                         \
    {                                                          \
        return staticType;                                     \
    }

namespace nf
{
class Object
{
public:
    Object() = default;
    virtual ~Object() = default;

    typeid_t typeID() const noexcept
    {
        return m_typeID;
    }

    inline UUID uuid() const noexcept
    {
        return m_uuid;
    }

    inline void assignTypeID(typeid_t id) noexcept
    {
        m_typeID = id;
    }

    inline void setUUID(UUID uuid) noexcept
    {
        m_uuid = uuid;
    }

    virtual bool onEvent(FlowEvent* event)
    {
        NF_UNUSED(event);
        return false;
    }

private:
    typeid_t m_typeID = 0;
    UUID m_uuid;
};
} // namespace nf