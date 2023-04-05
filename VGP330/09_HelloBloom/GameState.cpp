#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 0.0f, -15.0f });

	mDirectionalLight.ambient = Colors::Black;
	mDirectionalLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	
	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();
	mBaseRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mBloomRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(&mCamera);
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);

	mTexturingEffect.Initialize();
	mTexturingEffect.SetCamera(&mCamera);

	mGaussianBlurEffect.Initialize();
	mGaussianBlurEffect.SetSourceTexture(mBloomRenderTarget);
	mGaussianBlurEffect.SetBlurIterations(10);
	mGaussianBlurEffect.SetBlurSaturation(1.0f);

	mPostProcessingEffect.Initialize();
	mPostProcessingEffect.SetTexture(&mBaseRenderTarget, 0);
	mPostProcessingEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(), 1);
	
	auto textureManager = TextureManager::Get();

	mEarth.material.ambient = { 0.75f, 0.75f, 0.75f, 1.0f };
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

	mSun.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/sun.jpg");
	mSun.meshBuffer.Initialize(MeshBuilder::CreateSpherePX(5.0f, 256, 256));

	mScreenQuad.meshBuffer.Initialize(MeshBuilder::CreateNDCQuad());

	mEarthDistance = 8.0f;
}

void RAVEN::GameState::Termiante()
{
	
	mScreenQuad.Terminate();
	mSun.Terminate();
	mCloud.Terminate();
	mSkydome.Terminate();
	mEarth.Terminate();

	mPostProcessingEffect.Terminate();
	mGaussianBlurEffect.Terminate();
	mTexturingEffect.Terminate();
	mStandardEffect.Terminate();

	mBloomRenderTarget.Terminate();	
	mBaseRenderTarget.Terminate();
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

	//	Earth Rotation
	if (inputSystem->IsKeyDown(KeyCode::UP))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::XAxis, deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::XAxis, -deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, -deltaTime);
	
	//	Reset Camera Position
	if (inputSystem->IsKeyDown(KeyCode::R))
		mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

	mSkydome.transform.position = mCamera.GetPosition();

	mCloudRotation += deltaTime * 0.01f;

	mEarth.transform.position = TransformCoord({ 0.0f, 0.0f, -mEarthDistance }, Matrix4::RotationY(mEarthRevolution));
	mEarth.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, mEarthRotation);
	mEarthRevolution += mRevolutionIncrement;

	mCloud.transform = mEarth.transform;
	mCloud.transform.rotation *= Quaternion::RotationAxis(Vector3::YAxis, mCloudRotation);

	static float timer = 0.0f;
	timer += deltaTime;
	mPostProcessingEffect.SetTimer(timer);

}

void RAVEN::GameState::Render()
{
	mBaseRenderTarget.BeginRender();
		mTexturingEffect.Begin();
			mTexturingEffect.Render(mSkydome);
			mTexturingEffect.Render(mSun);
		mTexturingEffect.End();

		mStandardEffect.Begin();
			mStandardEffect.Render(mEarth);
			mStandardEffect.Render(mCloud);
		mStandardEffect.End();

		SimpleDraw::AddTransform(Matrix4::Identity);
		SimpleDraw::Render(mCamera);
	mBaseRenderTarget.EndRender();

	//	Draw brightness
	mBloomRenderTarget.BeginRender();
		mStandardEffect.Begin();
			auto originalMaterial = mEarth.material;
			mEarth.material.ambient = Colors::Black;
			mEarth.material.diffuse = Colors::Black;
			mEarth.material.specular = Colors::Black;
			mStandardEffect.Render(mEarth);
			mEarth.material = originalMaterial;
		mStandardEffect.End();
		mTexturingEffect.Begin();
			mTexturingEffect.Render(mSun);
		mTexturingEffect.End();
	mBloomRenderTarget.EndRender();
	
	//Apply Blur
	mGaussianBlurEffect.Begin();
		mGaussianBlurEffect.Render(mScreenQuad);
	mGaussianBlurEffect.End();

	mPostProcessingEffect.Begin();
	mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End(); 
	
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

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Earth", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Distance##Earth", &mEarthDistance, 0.1f);
		ImGui::DragFloat("Rotation##Earth", &mEarthRotation, 0.001f);
		ImGui::DragFloat("Revolution##Earth", &mEarthRevolution, 0.1f);
		ImGui::DragFloat("Revolution Increment##Increment", &mRevolutionIncrement, 0.001f);
	}
	
	
	mStandardEffect.DebugUI();
	mGaussianBlurEffect.DebugUI();
	mPostProcessingEffect.DebugUI();

	ImGui::End();


	ImGui::Begin("Render Targets", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Base Render");
	ImGui::Image(mBaseRenderTarget.GetRawData(), {256.0f, 144.0f});
	ImGui::Text("Bloom Render");
	ImGui::Image(mBloomRenderTarget.GetRawData(), { 256.0f, 144.0f });
	ImGui::Text("Horizontal Blur Render");
	ImGui::Image(mGaussianBlurEffect.GetHorizontalBlurTexture().GetRawData(), { 256.0f, 144.0f });
	ImGui::Text("Vertical Blur Render");
	ImGui::Image(mGaussianBlurEffect.GetVerticalBlurTexture().GetRawData(), { 256.0f, 144.0f });
	ImGui::End();

}
