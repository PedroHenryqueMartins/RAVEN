#pragma once

#include "TypeIds.h"
#include "RAVEN/Inc/RAVEN.h"

class AIComponent final : public RAVEN::Component
{
public:
	SET_TYPE_ID(ComponentId::AIComponent);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void DebugUI() override;

	void HandleEvent(const RAVEN::Event& evt) override;

	void SetMoveSpeed(float moveSpeed) { mMoveSpeed = moveSpeed; }

	//void HandleEvent(const Event& evt) override;

private:
	enum class State
	{
		Idle,
		Walk,
		Alert
	};

	enum class Anim
	{
		Default,
		Idle, 
		Walk,
		Alert
	};

	void UpdateIdle(float deltaTime);
	void UpdateWalk(float deltaTime);
	void UpdateAlert(float deltaTime);

	//const RAVEN::TerrainService* mTerrainService = nullptr;

	RAVEN::TransformComponent* mTransformComponent = nullptr;
	RAVEN::AnimatorComponent* mAnimatorComponent = nullptr;

	State mState = State::Idle;
	RAVEN::Math::Vector3 mDestination = RAVEN::Math::Vector3::Zero;
	

	float mWaitTime = 0.0f;
	float mMoveSpeed = 0.0f;
	float mWaitTimeAfterSeen = 0.0f;
};