#pragma once

#include "TypeIds.h"
#include "RAVEN/Inc/RAVEN.h"

class PlayerControllerComponent final : public RAVEN::Component
{
public:
	SET_TYPE_ID(ComponentId::PlayerControllerComponent);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;
	void HandleEvent(const RAVEN::Event& evt) override;

	const RAVEN::Math::Vector3& GetPosition() const { return mTransformComponent->position; }
	const RAVEN::Math::Vector3& GetDirection() const { return mDirection; }
	const RAVEN::Math::Vector3& GetVelocity() const { return mVelocity; }

	void SetVelocity(RAVEN::Math::Vector3 velocity) { mVelocity = velocity; }
	void SetDirection(RAVEN::Math::Vector3 direction) { mDirection = direction; }
	void SetMoveSpeed(float value) { mMoveSpeed = value; }

private:

	enum class Anim
	{
		Default,
		Idle,
		Run,
		Crouch,
		CrouchWalk
	};

	RAVEN::TransformComponent* mTransformComponent = nullptr;
	RAVEN::AnimatorComponent* mAnimatorComponent = nullptr;
	RAVEN::ColliderComponent* mColliderComponent = nullptr;
	RAVEN::SightComponent* mSightComponent = nullptr;

	RAVEN::Math::Vector3 mVelocity = RAVEN::Math::Vector3::Zero;
	RAVEN::Math::Vector3 mDirection = RAVEN::Math::Vector3::ZAxis;

	float mMoveSpeed = 0.0f;
	bool mCrouched = false;
	bool mIsMoving = false;
};