#pragma once

#include "Component.h"
#include "GameObjectHandle.h"

namespace RAVEN
{
	class GameWorld;

	class GameObject final
	{
	public:
		GameObject() = default;

		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		void HandleEvent(const Event& evt) const;

		template <class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject -- Components added to gameobjects must derive from Component class!");
			ASSERT(!mInitialized, "GameObject -- Cannot add components after GameObject has been initialized.");
			auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			return static_cast<ComponentType*>(newComponent.get());
		}

		template <class ComponentType>
		ComponentType* GetComponent()
		{
			auto constThis = static_cast<const GameObject*>(this);
			return const_cast<ComponentType*>(constThis->GetComponent<ComponentType>());
		}

		template <class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetTypeId() == ComponentType::StaticGetTypeId())
					return static_cast<ComponentType*>(component.get());
			}
			return nullptr;
		}

		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

		GameObjectHandle GetHandle() const { return mHandle; }

		void SetName(const char* name) { mName = name; }
		const std::string& GetName() const { return mName; }

	private:
		friend class GameWorld;
		using Components = std::vector<std::unique_ptr<Component>>;

		GameWorld* mWorld = nullptr;
		Components mComponents;
		GameObjectHandle mHandle;
		std::string mName = "Unnamed";
		bool mInitialized = false;
	};
}