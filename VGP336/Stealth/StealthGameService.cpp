#include "StealthGameService.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;
using namespace RAVEN::Physics;

void StealthGameService::Initialize()
{
	auto textureManager = TextureManager::Get();
	mGround.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mGround.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mGround.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mGround.material.power = 10.0f;
	mGround.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/dirt_seamless.jpg");
	mGround.meshBuffer.Initialize(MeshBuilder::CreatePlane(250.0f, 10, 10, 10.0f));
}

void StealthGameService::Terminate()
{

}

void StealthGameService::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		MainApp().Quit();
		return;
	}

	if (inputSystem->IsKeyPressed(KeyCode::F1))
		mShowDebugDraw = !mShowDebugDraw;
	
}

void StealthGameService::Render()
{
	
}

void StealthGameService::DebugUI()
{	
	if (mShowDebugDraw)
	{
		for (auto& gameObjects : GetWorld().GetUpdateList())
		{
			if (gameObjects)
				gameObjects->DebugUI();
		}
	}
}

void StealthGameService::GameOver()
{
	mGameOver = true;
}
