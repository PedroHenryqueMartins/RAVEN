#include "Precompiled.h"
#include "ColliderComponent.h"

#include "CollisionEvents.h"
#include "ColliderService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace RAVEN;

void RAVEN::ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();

	auto colliderService = GetOwner().GetWorld().GetService<ColliderService>();
	colliderService->Register(this);
}

void RAVEN::ColliderComponent::Terminate()
{
	auto colliderService = GetOwner().GetWorld().GetService<ColliderService>();
	colliderService->Unregister(this);
}

void ColliderComponent::DebugUI()
{
	const auto& aabb = GetWorldAABB();
	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, mColor);

	const auto& sphere = GetWorldSphere();
	Graphics::SimpleDraw::AddSphere(sphere.center, sphere.radius, mColor);
}

void ColliderComponent::HandleEvent(const Event& evt)
{
	switch (evt.GetTypeID())
	{
		case EventId::CollisionEnter:
		{
			//const OnCollisionEnterEvent& enterEvent = static_cast<const OnCollisionEnterEvent&>(evt);
			//enterEvent.other

			mColor = Graphics::Colors::Red;
			break;
		}
		case EventId::CollisionExit:
		{
			mColor = Graphics::Colors::Green;
			break;
		}
	}
}

Math::AABB RAVEN::ColliderComponent::GetWorldAABB() const
{
	return
	{
		mCenter + mTransformComponent->position,
		mExtend
	};
}

Math::AABB RAVEN::ColliderComponent::GetLocaldAABB() const
{
	return { mCenter, mExtend };
}

Math::Sphere RAVEN::ColliderComponent::GetLocalSphere() const
{
	const float radius = Math::Max(mExtend.x, mExtend.z);
	return { mCenter, radius };
}

Math::Sphere RAVEN::ColliderComponent::GetWorldSphere() const
{
	const float radius = Math::Max(mExtend.x, mExtend.z);
	return
	{
		mCenter + mTransformComponent->position,
		radius
	};
}