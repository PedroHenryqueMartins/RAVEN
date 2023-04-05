#include "Precompiled.h"
#include "ColliderService.h"

#include "ColliderComponent.h"
#include "SightComponent.h"
#include "CollisionEvents.h"
#include "SightEvents.h"
#include "GameObject.h"


using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

namespace
{
	std::pair<int64_t, int64_t> MakePair(const ColliderComponent* colliderComponentA, const ColliderComponent* colliderComponentB)
	{
		auto a = colliderComponentA->GetOwner().GetHandle().GetInt64();
		auto b = colliderComponentB->GetOwner().GetHandle().GetInt64();
		if (a > b)
			std::swap(a, b);
		return std::pair<int64_t, int64_t>(a, b);
	}

	std::pair<int64_t, int64_t> MakePair(const SightComponent* sightComponentA, const SightComponent* sightComponentB)
	{
		auto a = sightComponentA->GetOwner().GetHandle().GetInt64();
		auto b = sightComponentB->GetOwner().GetHandle().GetInt64();
		if (a > b)
			std::swap(a, b);
		return std::pair<int64_t, int64_t>(a, b);
	}
}

void ColliderService::Update(float deltaTime)
{
	for (int i = 0; i < mColliderEntries.size(); ++i)
	{
		for (int j = i + 1; j < mColliderEntries.size(); ++j)
		{
			auto colliderComponentA = mColliderEntries[i];
			auto colliderComponentB = mColliderEntries[j];
			const bool isColliding = IntersectSphere(colliderComponentA->GetWorldSphere(), colliderComponentB->GetWorldSphere());

			const auto pair = MakePair(colliderComponentA, colliderComponentB);
			const bool wasColliding = mCollisionPairs.find(pair) != mCollisionPairs.end();

			if (isColliding)
			{
				if (wasColliding)
				{
					//	OnCollisionStay
					colliderComponentA->GetOwner().HandleEvent(OnCollisionStayEvent(colliderComponentB));
					colliderComponentB->GetOwner().HandleEvent(OnCollisionStayEvent(colliderComponentA));
				}
				else
				{
					//	OnCollisionEnter
					colliderComponentA->GetOwner().HandleEvent(OnCollisionEnterEvent(colliderComponentB));
					colliderComponentB->GetOwner().HandleEvent(OnCollisionEnterEvent(colliderComponentA));
					mCollisionPairs.insert(pair);
				}
			}
			else if (wasColliding)
			{
				//	OnColisionExit
				colliderComponentA->GetOwner().HandleEvent(OnCollisionExitEvent(colliderComponentB));
				colliderComponentB->GetOwner().HandleEvent(OnCollisionExitEvent(colliderComponentA));
				mCollisionPairs.erase(pair);
			}

		}

	}

	for (int i = 0; i < mSightEntries.size(); ++i)
	{
		for (int j = i + 1; j < mSightEntries.size(); ++j)
		{
			auto sightComponentA = mSightEntries[i];
			auto sightComponentB = mSightEntries[j];
			const bool isSeeing = IntersectBox(sightComponentA->GetWorldAABB(), sightComponentB->GetWorldAABB());

			const auto pair = MakePair(sightComponentA, sightComponentB);
			const bool wasSeeing = mSightPairs.find(pair) != mSightPairs.end();

			if (isSeeing)
			{
				if (wasSeeing)
				{
					//	Staying on Sight
					sightComponentA->GetOwner().HandleEvent(OnSightStayEvent(sightComponentB));
					sightComponentB->GetOwner().HandleEvent(OnSightStayEvent(sightComponentA));
				}
				else
				{
					//	Entered sight
					sightComponentA->GetOwner().HandleEvent(OnSightEnterEvent(sightComponentB));
					sightComponentB->GetOwner().HandleEvent(OnSightEnterEvent(sightComponentA));
					mSightPairs.insert(pair);
				}
			}
			else if (wasSeeing)
			{
				sightComponentA->GetOwner().HandleEvent(OnSightExitEvent(sightComponentB));
				sightComponentB->GetOwner().HandleEvent(OnSightExitEvent(sightComponentA));
				mSightPairs.erase(pair);
			}
		}
	}
}

void ColliderService::Register(ColliderComponent* colliderComponent)
{
	mColliderEntries.push_back(colliderComponent);
}

void ColliderService::Unregister(ColliderComponent* colliderComponent)
{
	auto iter = std::find(mColliderEntries.begin(), mColliderEntries.end(), colliderComponent);
	if (iter != mColliderEntries.end())
	{
		mColliderEntries.erase(iter);
	}
}

void ColliderService::Register(SightComponent* sightComponent)
{
	mSightEntries.push_back(sightComponent);
}

void ColliderService::Unregister(SightComponent* sightComponent)
{
	auto iter = std::find(mSightEntries.begin(), mSightEntries.end(), sightComponent);
	if (iter != mSightEntries.end())
	{
		mSightEntries.erase(iter);
	}
}