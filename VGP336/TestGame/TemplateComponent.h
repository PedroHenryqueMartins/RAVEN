#pragma once

#include "TypeIds.h"
#include "RAVEN/Inc/RAVEN.h"

class TemplateComponent final : public RAVEN::Component
{
public:
	SET_TYPE_ID(ComponentId::Template);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	void SetMoveSpeed(float moveSpeed) { mMoveSpeed = moveSpeed; }

private:
	enum class State
	{
		Idle,
		Walk
	};

	enum class Anim
	{
		Default,
		Idle, 
		Walk
	};

	void UpdateIdle(float deltaTime);
	void UpdateWalk(float deltaTime);

	const RAVEN::TerrainService* mTerrainService = nullptr;

	RAVEN::TransformComponent* mTransformComponent = nullptr;
	RAVEN::AnimatorComponent* mAnimatorComponent = nullptr;

	State mState = State::Idle;
	RAVEN::Math::Vector3 mDestination = RAVEN::Math::Vector3::Zero;

	float mWaitTime = 0.0f;
	float mMoveSpeed = 0.0f;
};