#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FPSCameraControllerComponent.h"
#include "GroundPlaneComponent.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "SightComponent.h"
#include "ThirdPersonCameraControllerComponent.h"
#include "TransformComponent.h"

using namespace RAVEN;

namespace rj = rapidjson;

namespace
{
	CustomMake TryMake;
}

void RAVEN::GameObjectFactory::SetCustomMake(CustomMake customMake)
{
	TryMake = customMake;
}

void GameObjectFactory::Make(std::filesystem::path templateFile, GameObject& gameObject)
{

	FILE* file = nullptr;
	auto err = fopen_s(&file, templateFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameObjectFactory -- Failed to open template file '%s'", templateFile.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));

	fclose(file);

	rj::Document document;
	document.ParseStream(readStream);

	auto components = document["Components"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (TryMake && TryMake(componentName, component.value, gameObject))
			continue;
		if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			auto animatorComponent = gameObject.AddComponent<AnimatorComponent>();
		}
		else if (strcmp(componentName, "CameraComponent") == 0)
		{
			auto cameraComponent = gameObject.AddComponent<CameraComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& center = component.value["Position"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				cameraComponent->GetCamera().SetPosition({ x, y, z });
			}
			if (component.value.HasMember("LookAt"))
			{
				const auto& center = component.value["LookAt"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				cameraComponent->GetCamera().SetLookAt({ x, y, z });
			}
		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			auto colliderComponent = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				colliderComponent->SetCenter({ x, y, z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				const float x = extend[0].GetFloat();
				const float y = extend[1].GetFloat();
				const float z = extend[2].GetFloat();
				colliderComponent->SetExtend({ x, y, z });
			}
		}
		else if (strcmp(componentName, "SightComponent") == 0)
		{
			auto sightComponent = gameObject.AddComponent<SightComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				sightComponent->SetCenter({ x, y, z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				const float x = extend[0].GetFloat();
				const float y = extend[1].GetFloat();
				const float z = extend[2].GetFloat();
				sightComponent->SetExtend({ x, y, z });
			}
		}
		else if (strcmp(componentName, "FPSCameraControllerComponent") == 0)
		{
			auto fpsCameraController = gameObject.AddComponent<FPSCameraControllerComponent>();
			if (component.value.HasMember("MoveSpeed"))
			{
				const auto moveSpeed = component.value["MoveSpeed"].GetFloat();
				fpsCameraController->SetMoveSpeed(moveSpeed);
			}
			if (component.value.HasMember("TurnSpeed"))
			{
				const auto turnSpeed = component.value["TurnSpeed"].GetFloat();
				fpsCameraController->SetTurnSpeed(turnSpeed);
			}
		}

		else if (strcmp(componentName, "ThirdPersonCameraControllerComponent") == 0)
		{
			auto thirdpersonCameraController = gameObject.AddComponent<ThirdPersonCameraControllerComponent>();
			if (component.value.HasMember("MoveSpeed"))
			{
				const auto moveSpeed = component.value["MoveSpeed"].GetFloat();
				thirdpersonCameraController->SetMoveSpeed(moveSpeed);
			}
			if (component.value.HasMember("TurnSpeed"))
			{
				const auto turnSpeed = component.value["TurnSpeed"].GetFloat();
				thirdpersonCameraController->SetTurnSpeed(turnSpeed);
			}
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto modelComponent = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("FileName"))
			{
				const char* fileName = component.value["FileName"].GetString();
				modelComponent->SetFileName(fileName);
			}
			if (component.value.HasMember("Animations"))
			{
				const auto& animationName = component.value["Animations"].GetArray();
				for (const auto& animations : animationName)
				{
					const char* animation = animations.GetString();
					modelComponent->AddAnimation(animation);
				}
			}
		}
		else if (strcmp(componentName, "TransformComponent") == 0)
		{
			auto transformComponent = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transformComponent->position = { x, y, z };
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& position = component.value["Rotation"].GetArray();
				const float x = position[0].GetFloat() * Math::Constants::DegToRad;
				const float y = position[1].GetFloat() * Math::Constants::DegToRad;
				const float z = position[2].GetFloat() * Math::Constants::DegToRad;
				transformComponent->rotation = Math::Quaternion::RotationEuler({ x, y, z });
			}
			if (component.value.HasMember("Scale"))
			{
				const auto& position = component.value["Scale"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transformComponent->scale = { x, y, z };
			}
		}
		else if (strcmp(componentName, "GroundPlaneComponent") == 0)
		{
			auto templateComponent = gameObject.AddComponent<GroundPlaneComponent>();
			if (component.value.HasMember("Size"))
			{
				const float size = component.value["Size"].GetFloat();
				templateComponent->SetSize(size);
			}
		}
		else
		{
			ASSERT(false, "Unrecognized component %s", componentName);
		}

		// ... more components here ...
	}
}