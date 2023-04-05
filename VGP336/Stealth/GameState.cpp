#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

#include "AIComponent.h"
#include "PlayerControllerComponent.h"
#include "PreloadService.h"
#include "StealthGameService.h"

using namespace RAVEN;

namespace
{
	bool AddService(const char* serviceName, const rapidjson::Value& value, GameWorld& gameObject)
	{
		return false;
	}

	bool LoadCustomComponent(const char* componentName, const rapidjson::Value& value, GameObject& gameObject)
	{
		if (strcmp(componentName, "PlayerControllerComponent") == 0)
		{
			auto templateComponent = gameObject.AddComponent<PlayerControllerComponent>();
			if (value.HasMember("MoveSpeed"))
			{
				const float moveSpeed = value["MoveSpeed"].GetFloat();
				templateComponent->SetMoveSpeed(moveSpeed);
			}
			return true;
		}
		if (strcmp(componentName, "AIComponent") == 0)
		{
			auto templateComponent = gameObject.AddComponent<AIComponent>();
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

void GameState::CreateWorld()
{
	GameObjectFactory::SetCustomMake(LoadCustomComponent);
	//mGameWorld.LoadLevel("../../Assets/Levels/test_level.json");

	//	Spawn Player, camera and ground
	auto groundObj = mGameWorld.CreateGameObject("../../Assets/Templates/groundPlane.json");
	groundObj->SetName("Ground");
	auto playerObj = mGameWorld.CreateGameObject("../../Assets/Templates/playerController.json");
	playerObj->SetName("Player");
	playerObj->GetComponent<TransformComponent>()->position = { 25.0f, 0.0f, -15.0f };
	auto cameraObj = mGameWorld.CreateGameObject("../../Assets/Templates/thirdperson_camera.json");
	cameraObj->SetName("MainCamera");

	//	Spawn Walls
	for (int i = 0; i < 20; ++i)
	{
		auto topWall = mGameWorld.CreateGameObject("../../Assets/Templates/walls.json");
		auto leftWall = mGameWorld.CreateGameObject("../../Assets/Templates/walls.json");
		auto rightWall = mGameWorld.CreateGameObject("../../Assets/Templates/walls.json");
		auto bottomWall = mGameWorld.CreateGameObject("../../Assets/Templates/walls.json");
		rightWall->GetComponent<TransformComponent>()->rotation = Math::Quaternion::RotationAxis(Math::Vector3::YAxis, 1.57f);
		leftWall->GetComponent<TransformComponent>()->rotation = Math::Quaternion::RotationAxis(Math::Vector3::YAxis, 1.57f);
		std::string name = "Wall";
		//name += std::to_string(i);
		topWall->SetName(name.c_str());
		bottomWall->SetName(name.c_str());
		leftWall->SetName(name.c_str());
		rightWall->SetName(name.c_str());
		topWall->GetComponent<TransformComponent>()->position = { 2.0f * (float)i, 1.0f, 0.0f };
		bottomWall->GetComponent<TransformComponent>()->position = { 2.0f * (float)i, 1.0f, -40.0f };
		leftWall->GetComponent<TransformComponent>()->position = {0.0f, 1.0f, -2.0f * (float)i };
		rightWall->GetComponent<TransformComponent>()->position = { 40.0f, 1.0f, -2.0f * (float)i };
		
	}

	//	Spawn AI Agents
	for (int i = 0; i < 10; ++i)
	{
		float posX = Math::Random::UniformFloat(10.0f, 20.0f);
		float posZ = Math::Random::UniformFloat(-30.0f, -10.0f);
		auto gameObject = mGameWorld.CreateGameObject("../../Assets/Templates/aiModel.json");
		std::string name = "AI";
		//name += std::to_string(i);
		gameObject->SetName(name.c_str());
		gameObject->GetComponent<TransformComponent>()->position = { posX, 0.0f, posZ };
	}
}

void GameState::Initialize()
{
	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<ColliderService>();
	mGameWorld.AddService<PreloadService>();
	mGameWorld.AddService<StealthGameService>();
	mGameWorld.Initialize(1000);

	CreateWorld();
}

void GameState::Termiante()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
	if (mGameWorld.GetService<StealthGameService>()->isGameOver())
	{
		ImGui::Begin("GAME OVER");
		if (ImGui::Button("QUIT", ImVec2{ 500, 500 }))
		{
			MainApp().Quit();
		}
		ImGui::End();
	}
}