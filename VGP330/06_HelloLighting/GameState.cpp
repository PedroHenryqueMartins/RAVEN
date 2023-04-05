#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

	mDirectionalLight.ambient = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	
	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(&mCamera);
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);

	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(&mCamera);

	auto textureManager = TextureManager::Get();

	mEarth.material.ambient = Colors::Black;
	mEarth.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mEarth.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };  
	mEarth.material.power = 10.0f;
	mEarth.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/earth.jpg");
	mEarth.specularMapID = textureManager->LoadTexture("../../Assets/Images/earth_spec.jpg");
	mEarth.displacementMapID = textureManager->LoadTexture("../../Assets/Images/earth_bump.jpg");
	mEarth.normalMapID = textureManager->LoadTexture("../../Assets/Images/earth_normal.jpg");
	mEarth.meshBuffer.Initialize(MeshBuilder::CreateSphere(1.0f, 512, 512));

	mSkydome.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/skybox.jpg");
	mSkydome.depthTest = DepthStencilState::DepthTest::Disable;
	mSkydome.depthWrite = DepthStencilState::DepthWrite::Disable;
	mSkydome.meshBuffer.Initialize(MeshBuilder::CreateSkybox(10.0f, 256, 256));

	mCloud.material.ambient = { 0.75f, 0.75f, 0.75f, 1.0f };	
	mCloud.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mCloud.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mCloud.material.power = 10.0f;
	mCloud.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/earth_clouds.jpg");
	mCloud.blendMode = BlendState::Mode::Additive;
	mCloud.meshBuffer.Initialize(MeshBuilder::CreateSphere(1.2f, 256, 256));
}

void RAVEN::GameState::Termiante()
{
	
	mCloud.Terminate();
	mSkydome.Terminate();
	mEarth.Terminate();
	mTexturingEffect.Terminate();
	mStandardEffect.Terminate();
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

	mSkydome.transform.position = mCamera.GetPosition();

	//	Earth Rotation
	if (inputSystem->IsKeyDown(KeyCode::UP))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::XAxis, deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::XAxis, -deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, -deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::R))
		mEarth.transform.rotation = Quaternion::Identity;
	
	//	Reset Camera Position
	if (inputSystem->IsKeyDown(KeyCode::R))
		mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

	mCloud.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, deltaTime * 0.01f);

}

void RAVEN::GameState::Render()
{
	mTexturingEffect.Begin();
	mTexturingEffect.Render(mSkydome);
	mTexturingEffect.End();

	mStandardEffect.Begin();
	mStandardEffect.Render(mEarth);
	mStandardEffect.Render(mCloud);
	mStandardEffect.End();

	

	SimpleDraw::AddLine(Vector3::Zero, Vector3::XAxis, Colors::Red);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis, Colors::Green);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis, Colors::Blue);
	SimpleDraw::Render(mCamera);

}

void RAVEN::GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
	{
		mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
	}

	ImGui::Separator();

	ImGui::ColorEdit3("Light Ambient", &mDirectionalLight.ambient.r);
	ImGui::ColorEdit3("Light Diffuse", &mDirectionalLight.diffuse.r);
	ImGui::ColorEdit3("Light Specular", &mDirectionalLight.specular.r);

	ImGui::Separator();

	ImGui::ColorEdit3("Material Ambient", &mEarth.material.ambient.r);
	ImGui::ColorEdit3("Material Diffuse", &mEarth.material.diffuse.r);
	ImGui::ColorEdit3("Material Specular", &mEarth.material.specular.r);

	ImGui::Separator();

	ImGui::DragFloat("Power##Material", &mEarth.material.power, 0.1f, 1.0f, 100.0f);
	
	mStandardEffect.DebugUI();

	ImGui::End();

}
