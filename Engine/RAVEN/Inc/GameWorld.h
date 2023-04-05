#pragma once

#include "GameObject.h"
#include "Service.h"

namespace RAVEN
{
	class GameWorld final
	{
	public:
		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		template <class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld -- Services must derive from Service class!");
			ASSERT(!mInitialized, "GameWorld -- Cannot add Services after GameWorld has been initialized.");
			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template <class ServiceType>
		ServiceType* GetService()
		{
			auto constThis = static_cast<const GameWorld*>(this);
			return const_cast<ServiceType*>(constThis->GetService<ServiceType>());
		}

		template <class ServiceType>
		const ServiceType* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
					return static_cast<ServiceType*>(service.get());
			}
			return nullptr;
		}

		void LoadLevel(std::filesystem::path levelFile); 
		GameObject* CreateGameObject(std::filesystem::path templateFile);
		GameObject* GetGameObject(GameObjectHandle handle);
		GameObject* GetGameObject(const char* gameObjectName);
		void DestroyGameObject(GameObjectHandle handle);
		bool IsValid(const char* gameObjectName);

		const std::vector<GameObject*> GetUpdateList() const { return mUpdateList; }

	private:

		bool IsValid(GameObjectHandle handle) const;
		void ProcessDestroyList();

		//	GameObject destroy are solved by Deferred Deletion

		struct Slot
		{
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectSlots = std::vector<Slot>;
		using GameObjectPtrs = std::vector<GameObject*>;

		Services mServices;

		GameObjectSlots mGameObjectSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestroyed;

		GameObjectPtrs mUpdateList;

		bool mInitialized = false;
		bool mUpdating = false;
		bool mShowDebug = true;
	};
}