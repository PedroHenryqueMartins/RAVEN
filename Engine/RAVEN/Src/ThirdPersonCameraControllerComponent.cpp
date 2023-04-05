#include "Precompiled.h"
#include "ThirdPersonCameraControllerComponent.h"

#include "GameObject.h"
#include "CameraComponent.h"

#include "GameWorld.h"
#include "TransformComponent.h"

using namespace RAVEN;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void ThirdPersonCameraControllerComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
	mTargetToFollow = GetOwner().GetWorld().GetGameObject("Player")->GetHandle();
	if (mMoveSpeed == 0.0f)
		mMoveSpeed = 4.0f;
	if (mTurnSpeed == 0.0f)
		mTurnSpeed = 4.0f;
}

void ThirdPersonCameraControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	auto& camera = mCameraComponent->GetCamera();

	auto direction = Vector3::ZAxis;
	const float angle = atan2(direction.x, direction.z);
	Matrix4 rotation = Matrix4::RotationY(angle);
	camera.SetPosition(GetOwner().GetWorld().GetGameObject(mTargetToFollow)->GetComponent<TransformComponent>()->position + TransformNormal(positionOffset, rotation));
	camera.SetLookAt(GetOwner().GetWorld().GetGameObject(mTargetToFollow)->GetComponent<TransformComponent>()->position + TransformNormal(Vector3(0.0f, 1.0f, 2.0f), rotation));
}