#include "GameService.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;
using namespace RAVEN::Physics;

void GameService::Initialize()
{

}

void GameService::Terminate()
{

}

void GameService::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		MainApp().Quit();
		return;
	}

	if (inputSystem->IsKeyPressed(KeyCode::G))
	{
		float posX = Random::UniformFloat(100.0f, 110.0f);
		float posZ = Random::UniformFloat(100.0f, 110.0f);
		auto gameObject = GetWorld().CreateGameObject("../../Assets/Templates/model_test.json");
		gameObject->GetComponent<TransformComponent>()->position = { posX, 0.0f, 100.0f };
		mHandles.push_back(gameObject->GetHandle());

		if (mHandles.size() > 5)
		{
			auto handle = mHandles.front();
			mHandles.erase(mHandles.begin());
			GetWorld().DestroyGameObject(handle);
		}

		auto handle = mHandles.front();
		auto firstTemplate = GetWorld().GetGameObject(handle);
		if (firstTemplate)
			GetWorld().GetService<RenderService>()->SetShadowFocus(firstTemplate->GetComponent<TransformComponent>()->position);
	}
}

void GameService::Render()
{

}

void GameService::DebugUI()
{
	for (auto& handle : mHandles)
	{
		GameObject* gameObject = GetWorld().GetGameObject(handle);
		if (gameObject)
		{
			gameObject->DebugUI();
		}
	}
}
