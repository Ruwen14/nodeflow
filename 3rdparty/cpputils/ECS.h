#pragma once
#include <vector>
#include <unordered_map>

namespace ECS
{

	using Entity = uint32_t;
	using ComponentID = std::size_t;
	namespace details
	{
		struct UniqueIDCounter
		{
		protected:
			static ComponentID genID() noexcept {
				static ComponentID counter = 0;
				return counter++;
			}
		};
	}

	template<class Derived>
	struct ESCComponent : public details::UniqueIDCounter
	{
	public:
		static ComponentID id() noexcept
		{
			static ComponentID id_ = UniqueIDCounter::genID();
			return id_;
		}
	};

	struct ComponentContainerBase
	{
		~ComponentContainerBase() = default;
	};

	template<typename Component>
	struct ComponentContainer : public ComponentContainerBase
	{
		
		void add(const Component& c)
		{
			this->m_components.push_back(c);
		}


	private:
		std::vector<Component> m_components;
	};

	class ComponentManager
	{
	public:

		ComponentManager() = default;

		template<typename Component>
		void registerComponent()
		{
			static_assert(std::is_base_of<ESCComponent<Component>, Component>::value, "Component needs to be derived from ESCComponent.");
			ComponentID id = Component::id();
			m_componentContainers.insert({ id, std::make_unique<ComponentContainer<Component>>()});
		}
		template<typename Component>
		void addComponent()
		{
			static_assert(std::is_base_of<ESCComponent<Component>, Component>::value, "Component needs to be derived from ESCComponent.");
			//auto ar = static_cast<ComponentContainer<Component>*>(m_componentContainers.at(Component::id()).get());
			ComponentID id = Component::id();
			auto rawPtr = m_componentContainers.at(id).get();
			ComponentContainer<Component>* underlyingContainer = static_cast<ComponentContainer<Component>*>(rawPtr);
			//underlyingContainer->components.push_back()

			//auto ar = *m_componentContainers.at(id);
			//auto ar = m_componentContainers.at(Component::id())
			//auto ar = static_cast<ComponentContainer<Component>*>(m_componentContainers.at(Component::id()).get());
			//auto ar = *m_componentContainers.at(Component::id()).get();

			//ar.m_components
			
			//auto ar = static_cast<ComponentContainer<Component>>(m_componentContainers.at(Component::id()));
		}




	private:
		std::unordered_map < ComponentID, std::unique_ptr<ComponentContainerBase>> m_componentContainers;
	};
}
