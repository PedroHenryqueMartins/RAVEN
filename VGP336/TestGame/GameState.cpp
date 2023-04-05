#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

#include "TemplateComponent.h"
#include "GameService.h"
#include "PreloadService.h"

using namespace RAVEN;

namespace
{
	bool AddService(const char* serviceName, const rapidjson::Value& value, GameWorld& gameObject)
	{
		return false;
	}

	bool LoadCustomComponent(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "TemplateComponent") == 0)
		{
			auto templateComponent = gameObject.AddComponent<TemplateComponent>();
			if (value.HasMember("MoveSpeed"))
			{
				const float moveSpeed = value["MoveSpeed"].GetFloat();
				templateComponent->SetMoveSpeed(moveSpeed);
			}
			return true;
		}

		return false;
	}
}

void RAVEN::GameState::Initialize()
{
	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<ColliderService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<TerrainService>();
	mGameWorld.AddService<GameService>();
	mGameWorld.AddService<PreloadService>();
	mGameWorld.Initialize(1000);

	auto terrainService = mGameWorld.GetService<TerrainService>();
	terrainService->LoadTerrain("../../Assets/Heightmaps/heightmap_512x512.raw", 18.0f);
	terrainService->LoadTexture("../../Assets/Images/grass_2048.jpg", 0);
	terrainService->LoadTexture("../../Assets/Images/dirt_seamless.jpg", 1);

	GameObjectFactory::SetCustomMake(LoadCustomComponent);
	//mGameWorld.LoadLevel("../../Assets/Levels/test_level.json");
	mGameWorld.CreateGameObject("../../Assets/Templates/fps_camera.json"); 
}

void RAVEN::GameState::Termiante()
{
	mGameWorld.Terminate();
}

void RAVEN::GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void RAVEN::GameState::Render()
{
	mGameWorld.Render();
}

void RAVEN::GameState::DebugUI()
{
	mGameWorld.DebugUI();
}