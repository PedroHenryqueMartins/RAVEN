#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, 3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(&mCamera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.SetDepthBias(0.00001f);

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(&mDirectionalLight);

	auto textureManager = TextureManager::Get();

	mGroundPlane.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mGroundPlane.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mGroundPlane.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };  
	mGroundPlane.material.power = 10.0f;
	mGroundPlane.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/floor.jpg");
	mGroundPlane.meshBuffer.Initialize(MeshBuilder::CreatePlane(25.0f, 10, 10, 5.0f));

	mBall.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mBall.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mBall.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mBall.material.power = 10.0f;
	mBall.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/basketball.jpg");
	mBall.meshBuffer.Initialize(MeshBuilder::CreateSphere(1.0f, 128, 128));

	Model duckModel;
	ModelIO::LoadModel("../../Assets/Models/Duck/duck.model", duckModel);

	mDuck.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDuck.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDuck.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDuck.material.power = 10.0f;
	mDuck.diffuseMapID = textureManager->LoadTexture("../../Assets/Models/Duck/duck.png");
	mDuck.meshBuffer.Initialize(duckModel.meshData[0].mesh);

	mDuckAnimation = AnimationBuilder()
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 0.0f)
		.AddPositionKey({ 5.0f, 0.0f, 0.0f }, 1.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 2.0f)
		.AddPositionKey({ -5.0f, 0.0f, 0.0f }, 3.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 4.0f)
		.AddPositionKey({ 10.0f, 0.0f, 0.0f }, 5.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 6.0f)
		.AddPositionKey({ -10.0f, 0.0f, 0.0f }, 7.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 8.0f)
		.AddRotationKey(Quaternion::RotationEuler({ 0.0f, Constants::HalfPi, 0.0f }), 0.0f)
		.AddRotationKey(Quaternion::RotationEuler({ 0.0f, -Constants::HalfPi, 0.0f }), 2.0f)
		.AddRotationKey(Quaternion::RotationEuler({ 0.0f, Constants::HalfPi, 0.0f }), 4.0f)
		.AddScaleKey(Vector3::One, 0.0f)
		.AddScaleKey(Vector3{ 2.0f }, 1.0f)
		.AddScaleKey(Vector3::One, 2.0f)
		.AddScaleKey(Vector3{ 2.0f }, 3.0f)
		.AddScaleKey(Vector3{ 1.0f }, 4.0f)
		.Get();

	mBallAnimation = AnimationBuilder()
		.AddPositionKey({ 0.0f, 4.0f, 4.0f }, 0.0f)
		.AddPositionKey({ 0.0f, 1.0f, 4.0f }, 0.5f)
		.AddPositionKey({ 0.0f, 4.0f, 4.0f }, 1.0f)
		.AddRotationKey(Quaternion::Identity, 0.0f)
		.AddScaleKey({ Vector3::One }, 0.0f)
		.Get();


	Model hydroPlaneModel;
	ModelIO::LoadModel("../../Assets/Models/HydroPlane/HydroPlane.model", hydroPlaneModel);
	mHydroPlane.diffuseMapID = textureManager->LoadTexture("../../Assets/Models/HydroPlane/bixlerTexturedUVmap.png");
	mHydroPlane.meshBuffer.Initialize(hydroPlaneModel.meshData[0].mesh);
	mHydroPlane.transform.position.y = 5.0f;
	
	mPlaneAnimation = AnimationBuilder()
		.AddPositionKey({ 0.0f, 5.0f, 0.0f }, 0.0f)
		.AddPositionKey({ 0.0f, 30.0f, 50.0f }, 1.0f)
		.AddPositionKey({ 0.0f, 35.0f, 60.0f }, 2.0f)
		.AddPositionKey({ 0.0f, 30.0f, 70.0f }, 3.0f)
		.AddPositionKey({ 0.0f, 5.0f,  120.0f }, 4.0f)
		.AddRotationKey(Quaternion::Identity, 0.0f)
		.AddRotationKey(Quaternion::RotationAxis(Vector3::XAxis, Constants::Pi), 2.1f)
		.AddRotationKey(Quaternion::RotationAxis(Vector3::XAxis, 360.0f * Constants::DegToRad), 3.3f)
		.AddScaleKey(0.2f, 0.0f)
		.Get();

}

void RAVEN::GameState::Termiante()
{
	mDuck.Terminate();
	mBall.Terminate();
	mHydroPlane.Terminate();
	for (auto& parts : mPaladin)
		parts->Terminate();
	mGroundPlane.Terminate();

	mShadowEffect.Terminate();
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

	if (inputSystem->IsKeyPressed(KeyCode::PAUSE))
		mPause = !mPause;
	if (mPause)
		deltaTime = 0.0f;

	mBallAnimationTime += deltaTime;
	const float ballDuration = mBallAnimation.GetDuration();
	while (mBallAnimationTime > ballDuration)
		mBallAnimationTime -= ballDuration;

	mBall.transform.position = mBallAnimation.GetPosition(mBallAnimationTime);
	mBall.transform.rotation = mBallAnimation.GetRotation(mBallAnimationTime);
	mBall.transform.scale= mBallAnimation.GetScale(mBallAnimationTime);

	mDuckAnimationTime += deltaTime;
	const float duration = mDuckAnimation.GetDuration();
	while (mDuckAnimationTime > duration)
		mDuckAnimationTime -= duration;

	const Quaternion fix = Quaternion::RotationEuler({ 0.0f, -Constants::HalfPi, 0.0f });
	mDuck.transform.position = mDuckAnimation.GetPosition(mDuckAnimationTime);
	mDuck.transform.rotation = fix * mDuckAnimation.GetRotation(mDuckAnimationTime);
	mDuck.transform.scale = mDuckAnimation.GetScale(mDuckAnimationTime);

	mPlaneAnimationTime += deltaTime;
	const float planeDuration = mPlaneAnimation.GetDuration();
	while (mPlaneAnimationTime > planeDuration)
		mPlaneAnimationTime -= planeDuration;
	
	mHydroPlane.transform.position = mPlaneAnimation.GetPosition(mPlaneAnimationTime);
	mHydroPlane.transform.rotation = mPlaneAnimation.GetRotation(mPlaneAnimationTime);
	mHydroPlane.transform.scale = mPlaneAnimation.GetScale(mPlaneAnimationTime);
}

void RAVEN::GameState::Render()
{
	mShadowEffect.Begin();
		mShadowEffect.Render(mBall);
		mShadowEffect.Render(mDuck);
		mShadowEffect.Render(mHydroPlane);
	mShadowEffect.End();
	mStandardEffect.Begin();
		mStandardEffect.Render(mGroundPlane);
		mStandardEffect.Render(mBall);
		mStandardEffect.Render(mDuck);
		mStandardEffect.Render(mHydroPlane);
		for (auto& parts : mPaladin)
			mStandardEffect.Render(*parts);
	mStandardEffect.End();

	SimpleDraw::AddTransform(Matrix4::Identity);
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

	ImGui::ColorEdit3("Material Ambient", &mBall.material.ambient.r);
	ImGui::ColorEdit3("Material Diffuse", &mBall.material.diffuse.r);
	ImGui::ColorEdit3("Material Specular", &mBall.material.specular.r);

	ImGui::Separator();

	ImGui::DragFloat("Power##Material", &mBall.material.power, 0.1f, 1.0f, 100.0f);

	ImGui::Separator();
	
	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();

	ImGui::End();
}
