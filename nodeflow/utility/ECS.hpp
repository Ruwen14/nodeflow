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
#include "../3rdparty/entt/single_include/entt/entt.hpp"


namespace nf
{
	using Entity = entt::entity;

	class EntityManager
	{
	public:
		EntityManager() = default;
		
		Entity createEntity()
		{
			return m_registry.create();
		}

		template<typename Component, typename... Args>
		Entity createEntity(Args&&... args)
		{
			auto entity = createEntity();
			m_registry.emplace<Component>(entity, std::forward<Args>(args)...);
			return entity;
		}

		template<typename... Components>
		Entity createEntity(Components&&... comps)
		{
			auto entity = createEntity();
			(m_registry.emplace<Components>(entity, std::forward<Components>(comps)), ...);
			return entity;
		}
		template<typename... Components>
		auto entitiesWith()
		{
			return m_registry.view<Components...>();
		}

		template<typename... Components>
		auto hasComponents(Entity entity) const
		{
			return m_registry.all_of<Components...>(entity);
		}

		template<typename Component, typename... Args>
		decltype(auto) addComponent(Entity entity, Args &&...args)
		{
			return m_registry.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
		}

		template<typename... Components>
		decltype(auto) getComponents(Entity entity)
		{
			return m_registry.get<Components...>(entity);
		}


		template<typename Component, typename... Other>
		void removeComponents(Entity entity) 
		{
			m_registry.remove<Component, Other...>(entity);
		}


		template<typename Func>
		void forEach(Func func) const
		{
			m_registry.each(std::forward<Func>(func));
		}

		entt::registry& reg()
		{
			return m_registry;
		}

		auto count() const noexcept
		{
			return m_registry.size();
		}

		template<typename... Components>
		void clearAll()
		{
			m_registry.clear<Components...>();
		}


	private:
		entt::registry m_registry;

	};

}