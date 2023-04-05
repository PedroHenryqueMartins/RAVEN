#pragma once

#include "Component.h"

namespace RAVEN
{
	class CameraComponent;

	class FPSCameraControllerComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::FPSCameraController);

		void Initialize() override;
		void Update(float deltaTime) override;

		void SetMoveSpeed(float movespeed) { mMoveSpeed = movespeed; }
		void SetTurnSpeed(float turnspeed) { mTurnSpeed = turnspeed; }

	private:
		CameraComponent* mCameraComponent = nullptr;
		float mMoveSpeed = 0.0f;
		float mTurnSpeed = 0.0f;
	};
}