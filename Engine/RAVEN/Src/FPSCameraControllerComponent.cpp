#include "Precompiled.h"
#include "FPSCameraControllerComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

using namespace RAVEN;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void FPSCameraControllerComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();

	if (mMoveSpeed == 0.0f)
		mMoveSpeed = 4.0f;
	if (mTurnSpeed == 0.0f)
		mTurnSpeed = 4.0f;
}


void FPSCameraControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& camera = mCameraComponent->GetCamera();

	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(mMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-mMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(mMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-mMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		camera.Rise(mMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		camera.Rise(-mMoveSpeed * deltaTime);

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * mTurnSpeed * deltaTime * Constants::DegToRad);
		camera.Pitch(inputSystem->GetMouseMoveY() * mTurnSpeed * deltaTime * Constants::DegToRad);
	}
}
