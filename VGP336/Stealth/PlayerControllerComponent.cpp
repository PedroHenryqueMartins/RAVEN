#include "PlayerControllerComponent.h"

using namespace RAVEN;
using namespace RAVEN::Input;
using namespace RAVEN::Math;
using namespace RAVEN::Graphics;

void PlayerControllerComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
	mColliderComponent = GetOwner().GetComponent<ColliderComponent>();
	mSightComponent = GetOwner().GetComponent<SightComponent>();
	mTransformComponent->rotation = Quaternion::RotationAxis(Vector3::YAxis, Constants::Pi);
}

void PlayerControllerComponent::Terminate()
{
}

void PlayerControllerComponent::Update(float deltaTime)
{
	const float turnSpeed = 1.0f;
	auto inputSystem = InputSystem::Get();

	if (inputSystem->IsKeyDown(KeyCode::LCONTROL))
	{
		mCrouched = true;
		/*if (inputSystem->IsKeyPressed(KeyCode::LCONTROL))
		{
			
			if (mVelocity.z != 0 || mVelocity.x != 0)
			{
				mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::CrouchWalk);
				mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::CrouchWalk, true);
			}
			else
			{
				mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Crouch);
				mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Crouch, true);
			}
		}*/
		//mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Crouch, true);
		mColliderComponent->SetCenter(Vector3{ mColliderComponent->GetCenter().x, 0.2f, mColliderComponent->GetCenter().z });
		mSightComponent->SetCenter(Vector3{ mSightComponent->GetCenter().x, 0.2f, mSightComponent->GetCenter().z });
		mMoveSpeed = 50.0f;
	}
	else
	{
		mCrouched = false;
		mColliderComponent->SetCenter(Vector3{ mColliderComponent->GetCenter().x, 1.0f, mColliderComponent->GetCenter().z });
		mSightComponent->SetCenter(Vector3{ mSightComponent->GetCenter().x, 1.0f, mSightComponent->GetCenter().z });
		mMoveSpeed = 100.0f;
	}

	if (inputSystem->IsKeyPressed(KeyCode::W) || inputSystem->IsKeyPressed(KeyCode::S)
		|| inputSystem->IsKeyPressed(KeyCode::A) || inputSystem->IsKeyPressed(KeyCode::D))
	{
		mIsMoving = true;
	}
	else
		mIsMoving = false;

	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mIsMoving = true;
		
		mVelocity += mDirection * mMoveSpeed * deltaTime;
		mTransformComponent->rotation = Quaternion::RotationLook(-mVelocity);
	}

	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mIsMoving = true;
		
		mVelocity -= mDirection * mMoveSpeed * deltaTime;
		mTransformComponent->rotation = Quaternion::RotationLook(-mVelocity);
	}

	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mIsMoving = true;
		
		mVelocity += Cross(Vector3::YAxis, mDirection) * mMoveSpeed * deltaTime;
		mTransformComponent->rotation = Quaternion::RotationLook(-mVelocity);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mIsMoving = true;
		mVelocity -= Cross(Vector3::YAxis, mDirection) * mMoveSpeed * deltaTime;
		mTransformComponent->rotation = Quaternion::RotationLook(-mVelocity);
	}
	
	if (mIsMoving)
	{
		if (mCrouched)
		{
			mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::CrouchWalk);
			mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::CrouchWalk, true);
		}
		else
		{
			mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Run);
			mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Run, true);
		}
	}
	else
	{
		if (mCrouched)
		{
			mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Crouch);
			mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Crouch, true);
		}
		else
		{
			mAnimatorComponent->GetAnimator().SetAnimClip((int)Anim::Idle);
			mAnimatorComponent->GetAnimator().PlayAnimation((int)Anim::Idle, true);
		}
	}
	
	mTransformComponent->position += mVelocity * deltaTime;
	mVelocity *= 0.65f;	
}

void PlayerControllerComponent::DebugUI()
{
	ImGui::Begin("Test", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("VelocityX: %f", GetOwner().GetComponent<PlayerControllerComponent>()->GetVelocity().x);
	ImGui::Text("VelocityZ: %f", GetOwner().GetComponent<PlayerControllerComponent>()->GetVelocity().z);
	ImGui::End();
}

void PlayerControllerComponent::HandleEvent(const RAVEN::Event& evt)
{
	switch (evt.GetTypeID())
	{
		case EventId::CollisionEnter:
		{
			const OnCollisionEnterEvent& enterEvent = static_cast<const OnCollisionEnterEvent&>(evt);
			if (enterEvent.other && strcmp(enterEvent.other->GetOwner().GetName().c_str(), "Wall") == 0)
			{
				const auto otherTransformComponent = enterEvent.other->GetOwner().GetComponent<TransformComponent>();

				if (otherTransformComponent->position.z > mTransformComponent->position.z)
				{
					mTransformComponent->position.z = otherTransformComponent->position.z - 2.0f;
					mTransformComponent->position.x = otherTransformComponent->position.x - 2.0f;
				}
				else
				{
					mTransformComponent->position.z = otherTransformComponent->position.z + 2.0f;
					mTransformComponent->position.x = otherTransformComponent->position.x + 2.0f;
				}
			}
			break;
		}
	}
}
