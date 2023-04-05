#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

enum mHeroAnimState
{
	Default,
	Idle,
	Walking,
	Jump,
	Kick,
	Punch
};

void RAVEN::GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, 3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.diffuse = Colors::White;
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

	auto modelManager = ModelManager::Get();

	//const char* mHeroFileName = "../../Assets/Models/Hero/hero.model";
	const char* mHeroFileName = "../../Assets/Models/CharController/charController.model";
	mHeroModelID = modelManager->LoadModel(mHeroFileName);

	/*modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/Hero/hero_idle");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/Hero/hero_walk");	
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/Hero/hero_jump");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/Hero/hero_kick");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/Hero/hero_punch");*/

	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerIdle");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerForward");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerForwardRight");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerRight");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerBackwardRight");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerBackward");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerBackwardLeft");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerForwardLeft");
	modelManager->AddAnimation(mHeroModelID, "../../Assets/Models/CharController/charControllerLeft");


	//mHeroAnimator.Initialize(mHeroModelID);
	//mHeroAnimator.PlayAnimation(mHeroAnimator.Idle, true);

	mCharController.Initialize(mHeroModelID);

	const Model* mHeroModel = modelManager->GetModel(mHeroModelID);

	mRenderGroup = CreateRenderGroup(*mHeroModel, &mCharController.GetAnimator());
	
}

void RAVEN::GameState::Termiante()
{
	CleanUpRenderGroup(mRenderGroup);
	mGroundPlane.Terminate();

	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
}

void RAVEN::GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	auto graphiSystem = GraphicsSystem::Get();
	inputSystem->SetMouseClipToWindow(true);
	
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		MainApp().Quit();
		return;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	/*if (inputSystem->IsKeyDown(KeyCode::W))
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
	}*/

	mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);

	if (inputSystem->IsKeyPressed(KeyCode::PAUSE))
		mPause = !mPause;
	if (mPause)
		deltaTime = 0.0f;

	mCharController.Update(deltaTime);
	mCharController.SetDirection(mCamera.GetDirection());
	//mHeroAnimator.Update(deltaTime);
	//mHeroAnimator.SetDirection(mCamera.GetDirection());
	

	for (auto& renderObject : mRenderGroup)
	{
		renderObject.transform.position.x = mCharController.GetPosition().x;
		renderObject.transform.position.z = mCharController.GetPosition().z;
		renderObject.transform.rotation.y = -mCharController.GetDirection().x;
	}

	mCamera.SetPosition(mRenderGroup[0].transform.position + Vector3(0.0f, 2.0f, 1.0f));
	
}

void RAVEN::GameState::Render()
{
	
	mShadowEffect.Begin();
		DrawRenderGroup(mShadowEffect, mRenderGroup);
		
	mShadowEffect.End();
	mStandardEffect.Begin();
		if (mDrawSkeleton)
		{
			const auto modelManager = ModelManager::Get();
			{
				const Model* mawMonsterModel = modelManager->GetModel(mHeroModelID);
				const Skeleton& mawSkeleton = *mawMonsterModel->skeleton.get();

				AnimationUtil::BoneTransforms boneTransforms;
				AnimationUtil::ComputeBoneTransforms(mawSkeleton, [&](const Bone* bone) { return mHeroAnimator.GetToParentTransform(bone); }, boneTransforms);
				AnimationUtil::DrawSkeleton(mawSkeleton, boneTransforms);
			}
		}
		else
		{
			DrawRenderGroup(mStandardEffect, mRenderGroup);
			
		}
		mStandardEffect.Render(mGroundPlane);
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
	
	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();

	ImGui::Separator();

	ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);

	ImGui::Separator();

	

	ImGui::End();
}
