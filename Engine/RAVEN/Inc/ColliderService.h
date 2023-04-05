#pragma once

#include "Service.h"

namespace RAVEN
{
	class CameraService;

	class GameObjectHandle;
	class GameObject;
	class ColliderComponent;
	class SightComponent;

	class ColliderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Collision);

		void Update(float deltaTime) override;
		
	private:
		friend class ColliderComponent;
		friend class SightComponent;

		void Register(ColliderComponent* colliderComponent);
		void Unregister(ColliderComponent* colliderComponent);

		void Register(SightComponent* sightComponent);
		void Unregister(SightComponent* sightComponent);

		std::vector<ColliderComponent*> mColliderEntries;
		std::set<std::pair<int64_t, int64_t>> mCollisionPairs;

		std::vector<SightComponent*> mSightEntries;
		std::set<std::pair<int64_t, int64_t>> mSightPairs;
	};
}
