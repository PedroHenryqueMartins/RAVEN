#pragma once

#include "Component.h"

#include "GameObjectHandle.h"

namespace RAVEN
{
	class CameraComponent;

	class ThirdPersonCameraControllerComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::ThirdPersonCameraController);

		void Initialize() override;
		void Update(float deltaTime) override;

		void SetMoveSpeed(float movespeed) { mMoveSpeed = movespeed; }
		void SetTurnSpeed(float turnspeed) { mTurnSpeed = turnspeed; }

	private:
		CameraComponent* mCameraComponent = nullptr;
		GameObjectHandle mTargetToFollow;
		float mMoveSpeed = 0.0f;
		float mTurnSpeed = 0.0f;

		Math::Vector3 positionOffset = { 0.0f, 4.5, -3.0f };
	};
}