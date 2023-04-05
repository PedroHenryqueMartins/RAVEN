#include "AIComponent.h"

#include "StealthGameService.h"



using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

void AIComponent::Initialize()
{
	//mTerrainService = GetOwner().GetWorld().GetService<TerrainService>();

	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
}

void AIComponent::Terminate()
{

}

void AIComponent::Update(float deltaTime)
{
	switch (mState)
	{
	case State::Idle:
		UpdateIdle(deltaTime);
		break;
	case State::Walk:
		UpdateWalk(deltaTime);
		break;
	case State::Alert:
		UpdateAlert(deltaTime);
		break;
	}
}

void AIComponent::DebugUI()
{

}

void AIComponent::HandleEvent(const Event& evt)
{
	switch (evt.GetTypeID())
	{
		case EventId::CollisionEnter:
		{
			const OnCollisionEnterEvent& enterEvent = static_cast<const OnCollisionEnterEvent&>(evt);
			if (enterEvent.other && strcmp(enterEvent.other->GetOwner().GetName().c_str(), "Ground") != 0)
			{
				const auto otherTransformComponent = enterEvent.other->GetOwner().GetComponent<TransformComponent>();
				{
					if (otherTransformComponent->position.z > mTransformComponent->position.z)
					{
						mDestination = mTransformComponent->position;
						mDestination.z -= 5.0f;
						mDestination.z += Math::Random::UniformFloat(-10.0f, 10.0f);
						mDestination.x += Math::Random::UniformFloat(-10.0f, 10.0f);
					}
					else
					{
						mDestination = mTransformComponent->position;
						mDestination.z += 5.0f;
						mDestination.z += Math::Random::UniformFloat(-10.0f, 10.0f);
						mDestination.x += Math::Random::UniformFloat(-10.0f, 10.0f);
					}
				}
			}
			break;
		}

		case EventId::SightEnter:
		{
			//&& enterEvent.other->GetOwner().GetName().find("Wall") != std::string::npos
			const OnSightEnterEvent& enterEvent = static_cast<const OnSightEnterEvent&>(evt);
			if (strcmp(enterEvent.other->GetOwner().GetName().c_str(), "Wall") == 0 || strcmp(enterEvent.other->GetOwner().GetName().c_str(), "AI") == 0)
			{
				GetOwner().GetComponent<SightComponent>()->SetExtend(Vector3{ 0.3f, 0.2f, 0.1f });
			}
			break;
		}

		case EventId::SightExit:
		{
			const OnSightExitEvent& exitEvent = static_cast<const OnSightExitEvent&>(evt);
			if (exitEvent.other)
			{
				GetOwner().GetComponent<SightComponent>()->SetExtend(Vector3{ 0.3f, 0.2f, 2.0f });
			}
			break;
		}
	}
}

void AIComponent::UpdateIdle(float deltaTime)
{
	mWaitTime -= deltaTime;
	if (mWaitTime <= 0.0f)
	{
		mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Walk);
		mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Walk, true);
		mState = State::Walk;
		mDestination = mTransformComponent->position;
		mDestination.z += Math::Random::UniformFloat(-10.0f, 10.0f);
		mDestination.x += Math::Random::UniformFloat(-10.0f, 10.0f);

	}

	if (GetOwner().GetComponent<SightComponent>()->Seen())
	{
		mState = State::Alert;
		mWaitTimeAfterSeen = 2.0f;
		mTransformComponent->rotation = Quaternion::RotationLook(-GetOwner().GetWorld().GetGameObject("Player")->GetComponent<TransformComponent>()->position, Vector3::YAxis);
		mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Alert);
		mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Alert, true);
	}
}

void AIComponent::UpdateWalk(float deltaTime)
{
	if (Math::DistanceSqr(mTransformComponent->position, mDestination) < 1.0f)
	{
		mState = State::Idle;
		mWaitTime = 5.0f;
		mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Idle);
		mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Idle, true);
	}
	else
	{
		auto direction = Math::Normalize(mDestination - mTransformComponent->position);
		mTransformComponent->position += direction * mMoveSpeed * deltaTime;
		mTransformComponent->rotation = Quaternion::RotationLook(-direction, Vector3::YAxis);
	}

	if (GetOwner().GetComponent<SightComponent>()->Seen())
	{
		mState = State::Alert;
		mWaitTimeAfterSeen = 2.0f;
		mTransformComponent->rotation = Quaternion::RotationLook(-GetOwner().GetWorld().GetGameObject("Player")->GetComponent<TransformComponent>()->position, Vector3::YAxis);
		mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Alert, true);
	}
}

void AIComponent::UpdateAlert(float deltaTime)
{
	mWaitTimeAfterSeen -= deltaTime;
	if (mWaitTimeAfterSeen <= 0.0f)
	{
		GetOwner().GetWorld().GetService<StealthGameService>()->GameOver();
	}
}
