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

#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "nodeflow/reflection/type_reflection.hpp"
#include "nodeflow/typedefs.hpp"
#include "nodeflow/utility/Singleton.hpp"

namespace nf
{
class TypenameAtlas : public Singleton<TypenameAtlas>
{
public:
    using atlas_t = std::unordered_map<typeid_t, std::string>;

public:
    TypenameAtlas() = default;

    template <typename Type>
    bool add()
    {
        auto id = type_id<Type>();
        auto typeName = type_name<Type>();
        if (m_atlas.contains(id))
        {
            NF_ASSERT(m_atlas.at(id) == typeName,
                      "Hash collision of function: type_name<T>() detected. "
                      "Please fix affected type name");
            return false;
        }
        m_atlas[id] = typeName;
        return true;
    }

    template <typename Type>
    inline const std::string& get() const
    {
        return m_atlas.at(type_id<Type>());
    }

    inline const std::string& get(typeid_t id) const
    {
        return m_atlas.at(id);
    }

    inline bool contains(typeid_t id) const
    {
        return m_atlas.contains(id);
    }

    inline const atlas_t& atlas() const
    {
        return m_atlas;
    }

    std::set<std::string> listTypenames() const;

    bool hasDuplicates() const;

private:
    atlas_t m_atlas{};
};
} // namespace nf