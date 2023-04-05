#include "Precompiled.h"
#include "SightComponent.h"

#include "SightEvents.h"
#include "ColliderService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

void SightComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	
	auto colliderService = GetOwner().GetWorld().GetService<ColliderService>();
	colliderService->Register(this);
}

void SightComponent::Terminate()
{
	auto colliderService = GetOwner().GetWorld().GetService<ColliderService>();
	colliderService->Unregister(this);
}

void SightComponent::DebugUI()
{
	const auto& aabb = GetWorldAABB();
	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, mColor);
}

void SightComponent::HandleEvent(const Event& evt)
{
	switch (evt.GetTypeID())
	{
		case EventId::SightEnter:
		{
			const OnSightEnterEvent& enterEvent = static_cast<const OnSightEnterEvent&>(evt);
			if (strcmp(enterEvent.other->GetOwner().GetName().c_str(), "Player") == 0)
			{
				mColor = Graphics::Colors::Red;
				mSeenPlayer = true;
			}
			else
				mColor = Graphics::Colors::Magenta;
			break;
		}
		
		case EventId::SightExit:
		{
			mColor = Graphics::Colors::Blue;
			break;
		}
	}
}

RAVEN::Math::AABB SightComponent::GetWorldAABB() const
{
	return
	{
		mCenter + mTransformComponent->position,
		mExtend
	};
}

RAVEN::Math::AABB SightComponent::GetLocalAABB() const
{
	return { mCenter, mExtend };
}
