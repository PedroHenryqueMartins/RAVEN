#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, -15.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.diffuse = { 0.85f, 0.85f, 0.85f, 0.85f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();
	mOutlineRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(&mCamera);
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);

	mToonEffect.Initialize();
	mToonEffect.SetCamera(&mCamera);
	mToonEffect.SetDirectionalLight(&mDirectionalLight);

	mOutlineEffect.Initialize();
	mOutlineEffect.SetTexture(&mOutlineRenderTarget, 0);

	auto textureManager = TextureManager::Get();

	mGroundPlane.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mGroundPlane.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mGroundPlane.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };  
	mGroundPlane.material.power = 10.0f;
	mGroundPlane.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/floor.jpg");
	mGroundPlane.meshBuffer.Initialize(MeshBuilder::CreatePlane(25.0f, 10, 10, 5.0f));

	//	Toon Object
	Model objModel;
	ModelIO::LoadModel("../../Assets/Models/Duck/duck.model", objModel);
	for (uint32_t i = 0; i < objModel.meshData.size(); ++i)
	{
		auto& renderObj = mObject.emplace_back(std::make_unique<RenderObject>());
		renderObj->diffuseMapID = textureManager->LoadTexture("../../Assets/Models/duck/duck.png");
		renderObj->meshBuffer.Initialize(objModel.meshData[i].mesh);
	}

	//	Eevee
	Model objModel2;
	ModelIO::LoadModel("../../Assets/Models/Eevee/eevee.model", objModel2);
	for (uint32_t i = 0; i < objModel2.meshData.size(); ++i)
	{
		auto& renderObj = mObject2.emplace_back(std::make_unique<RenderObject>());
		if (i == 0)
		{
			renderObj->diffuseMapID = textureManager->LoadTexture("../../Assets/Models/Eevee/eevee_diffuse.png");
		}
		else if (i == 1)
		{
			renderObj->diffuseMapID = textureManager->LoadTexture("../../Assets/Models/Eevee/eevee_eye.png");
		}
		else
		{
			renderObj->diffuseMapID = textureManager->LoadTexture("../../Assets/Models/Eevee/eevee_mouth.png");
		}
		renderObj->meshBuffer.Initialize(objModel2.meshData[i].mesh);
		renderObj->transform.position = { 5.0f, 0.0f, 0.0f };
		renderObj->transform.rotation = Quaternion::RotationEuler({ 3.0f, 0.0f, 0.0f });
	}

	mOutlineScreen.meshBuffer.Initialize(MeshBuilder::CreateNDCQuad());
}

void RAVEN::GameState::Termiante()
{
	for (auto& parts : mObject)
		parts->Terminate();
	for (auto& parts : mObject2)
		parts->Terminate();
	mGroundPlane.Terminate();
	mOutlineScreen.Terminate();

	mOutlineEffect.Terminate();
	mToonEffect.Terminate();
	mStandardEffect.Terminate();
	
	mOutlineRenderTarget.Terminate();
}

void RAVEN::GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		MainApp().Quit();
		return;
	}
	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	const float time = Core::TimeUtil::GetTimeSeconds();
}

void RAVEN::GameState::Render()
{
	mOutlineRenderTarget.BeginRender();
	mStandardEffect.Begin();
	mStandardEffect.Render(mGroundPlane);
	mStandardEffect.End();

	mToonEffect.Begin();
	for (auto& parts : mObject)
		mToonEffect.Render(*parts);
	mToonEffect.Begin();
	for (auto& parts : mObject2)
		mToonEffect.Render(*parts);
	mToonEffect.End();
	mOutlineRenderTarget.EndRender();

	mOutlineEffect.Begin();
	mOutlineEffect.Render(mOutlineScreen);
	mOutlineEffect.End();
}

void RAVEN::GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
	{
		mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
	}

	ImGui::Separator();

	ImGui::ColorEdit3("Light Diffuse", &mDirectionalLight.diffuse.r);

	ImGui::Separator();
	mOutlineEffect.DebugUI();
	mToonEffect.DebugUI();

	ImGui::End();
}