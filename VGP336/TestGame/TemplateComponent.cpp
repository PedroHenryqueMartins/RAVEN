#include "TemplateComponent.h"

using namespace RAVEN;

void TemplateComponent::Initialize()
{
	mTerrainService = GetOwner().GetWorld().GetService<TerrainService>();
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
}

void TemplateComponent::Terminate()
{

}

void TemplateComponent::Update(float deltaTime)
{
	switch (mState)
	{
	case State::Idle:
		UpdateIdle(deltaTime);
		break;
	case State::Walk:
		UpdateWalk(deltaTime);
		break;
	}

	//	Snap to terrain
	const float height = mTerrainService->GetHeight(mTransformComponent->position);
	mTransformComponent->position.y = height;
}

void TemplateComponent::DebugUI()
{

}

void TemplateComponent::UpdateIdle(float deltaTime)
{
	static bool right = true;

	mWaitTime -= deltaTime;
	if (mWaitTime <= 0.0f)
	{
		mState = State::Walk;
		mDestination = mTransformComponent->position;
		//mDestination.z -= 10.0f;
		mDestination.x += right ? 5.0f : -5.0f;
		right = !right;
		mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Walk, true);
	}
}

void TemplateComponent::UpdateWalk(float deltaTime)
{
	if (Math::DistanceSqrXZ(mTransformComponent->position, mDestination) < 1.0f)
	{
		mState = State::Idle;
		mWaitTime = 2.0f;
		mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Idle, true);
	}
	else
	{
		auto direction = Math::NormalizeXZ(mDestination - mTransformComponent->position);
		mTransformComponent->position += direction * mMoveSpeed * deltaTime;
	}
}
