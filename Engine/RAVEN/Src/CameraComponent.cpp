#include "Precompiled.h"
#include "CameraComponent.h"

#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"

using namespace RAVEN;

void CameraComponent::Initialize()
{
	//	TODO: FINISH THIS INITIALIZE AND TERMINATE
	auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Register(this);
}

void CameraComponent::Terminate()
{
	auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Unregister(this);
}