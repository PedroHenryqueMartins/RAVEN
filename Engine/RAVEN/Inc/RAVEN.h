#pragma once

#include "Common.h"

//	App Headers
#include "App.h"
#include "AppState.h"

//	Component Headers
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FPSCameraControllerComponent.h"
#include "ModelComponent.h"
#include "SightComponent.h"
#include "ThirdPersonCameraControllerComponent.h"
#include "TransformComponent.h"

//	Event Headers
#include "CollisionEvents.h"
#include "SightEvents.h"

//	Service Headers
#include "CameraService.h"
#include "ColliderService.h"
#include "RenderService.h"
#include "TerrainService.h"

//	World Headers
#include "Component.h"
#include "Event.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"
#include "Service.h"
#include "TypeIds.h"

namespace RAVEN
{
	App& MainApp();
}