#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 5.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.diffuse = Colors::White;
	mDirectionalLight.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();
	gs->SetClearColor(Colors::LightSkyBlue);
	mOutlineRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(&mCamera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.SetDepthBias(0.00001f);

	mToonEffect.Initialize();
	mToonEffect.SetCamera(&mCamera);
	mToonEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mToonEffect.SetDirectionalLight(&mDirectionalLight);
	mOutlineEffect.Initialize();
	mOutlineEffect.SetTexture(&mOutlineRenderTarget, 0);

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

	const char* mawMonsterFileName = "../../Assets/Models/MawMonster/mawmonster.model";
	mMawMonsterModelID = modelManager->LoadModel(mawMonsterFileName);

	modelManager->AddAnimation(mMawMonsterModelID, "../../Assets/Models/MawMonster/mawmonster_ymca");
	modelManager->AddAnimation(mMawMonsterModelID, "../../Assets/Models/MawMonster/mawmonster_thriller_2");
	modelManager->AddAnimation(mMawMonsterModelID, "../../Assets/Models/MawMonster/mawmonster_gangnamStyle");

	mMawMonsterAnimator.Initialize(mMawMonsterModelID);
	mMawMonsterAnimator.PlayAnimation(animationIndex, true);

	const Model* mawMonsterModel = modelManager->GetModel(mMawMonsterModelID);
	const Model* mawMonsterModel2 = modelManager->GetModel(mMawMonsterModelID);
	const Model* mawMonsterModel3 = modelManager->GetModel(mMawMonsterModelID);
	const Model* mawMonsterModel4 = modelManager->GetModel(mMawMonsterModelID);
	const Model* mawMonsterModel5 = modelManager->GetModel(mMawMonsterModelID);

	mMawMonsterRG.push_back(CreateRenderGroup(*mawMonsterModel, &mMawMonsterAnimator));
	mMawMonsterRG.push_back(CreateRenderGroup(*mawMonsterModel2, &mMawMonsterAnimator));
	mMawMonsterRG.push_back(CreateRenderGroup(*mawMonsterModel3, &mMawMonsterAnimator));
	mMawMonsterRG.push_back(CreateRenderGroup(*mawMonsterModel4, &mMawMonsterAnimator));
	mMawMonsterRG.push_back(CreateRenderGroup(*mawMonsterModel5, &mMawMonsterAnimator));

	mMawMonsterRG[0].back().transform.position.x = 2.0f;
	mMawMonsterRG[0].back().transform.position.z = -2.0f;
	mMawMonsterRG[1].back().transform.position.x = 4.0f;
	mMawMonsterRG[2].back().transform.position.x = 0.0f;
	mMawMonsterRG[3].back().transform.position.x = -4.0f;
	mMawMonsterRG[4].back().transform.position.x = -2.0f;
	mMawMonsterRG[4].back().transform.position.z = -2.0f;

	animSize = mawMonsterModel->animationClips.size();

	mOutlineScreen.meshBuffer.Initialize(MeshBuilder::CreateNDCQuad());
}

void RAVEN::GameState::Termiante()
{
	for (auto& model : mMawMonsterRG)
		CleanUpRenderGroup(model);
	mGroundPlane.Terminate();
	mOutlineScreen.Terminate();

	mOutlineEffect.Terminate();
	mShadowEffect.Terminate();
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

	if (inputSystem->IsKeyPressed(KeyCode::PAUSE))
		mPause = !mPause;
	if (mPause)
		deltaTime = 0.0f;

	mMawMonsterAnimator.Update(deltaTime);
}

void RAVEN::GameState::Render()
{
	if (mUseToon)
	{
		mOutlineRenderTarget.BeginRender(Colors::LightSkyBlue);
		mShadowEffect.Begin();
		for (auto& model : mMawMonsterRG)
		{
			DrawRenderGroup(mShadowEffect, model);
		}

		mShadowEffect.End();
		mToonEffect.Begin();
		if (mDrawSkeleton)
		{
			const auto modelManager = ModelManager::Get();
			{
				const Model* mawMonsterModel = modelManager->GetModel(mMawMonsterModelID);
				const Skeleton& mawSkeleton = *mawMonsterModel->skeleton.get();

				AnimationUtil::BoneTransforms boneTransforms;
				AnimationUtil::ComputeBoneTransforms(mawSkeleton, [&](const Bone* bone) { return mMawMonsterAnimator.GetToParentTransform(bone); }, boneTransforms);
				AnimationUtil::DrawSkeleton(mawSkeleton, boneTransforms);
			}
		}
		else
		{
			for (auto& model : mMawMonsterRG)
				DrawRenderGroup(mToonEffect, model);
		}

		mToonEffect.End();
		mStandardEffect.Begin();
		mStandardEffect.Render(mGroundPlane);
		mStandardEffect.End();
		mOutlineRenderTarget.EndRender();

		mOutlineEffect.Begin();
		mOutlineEffect.Render(mOutlineScreen);
		mOutlineEffect.End();
	}
	else
	{
		mShadowEffect.Begin();
		for (auto& model : mMawMonsterRG)
			DrawRenderGroup(mShadowEffect, model);
		mShadowEffect.End();
		mStandardEffect.Begin();
		if (mDrawSkeleton)
		{
			const auto modelManager = ModelManager::Get();
			{
				const Model* mawMonsterModel = modelManager->GetModel(mMawMonsterModelID);
				const Skeleton& mawSkeleton = *mawMonsterModel->skeleton.get();

				AnimationUtil::BoneTransforms boneTransforms;
				AnimationUtil::ComputeBoneTransforms(mawSkeleton, [&](const Bone* bone) { return mMawMonsterAnimator.GetToParentTransform(bone); }, boneTransforms);
				AnimationUtil::DrawSkeleton(mawSkeleton, boneTransforms);
			}
		}
		else
		{
			for (auto& model : mMawMonsterRG)
				DrawRenderGroup(mStandardEffect, model);
		}
		mStandardEffect.Render(mGroundPlane);
		mStandardEffect.End();
	}

	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(mCamera);
}

void RAVEN::GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}

		ImGui::Separator();

		ImGui::ColorEdit3("Light Ambient", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit3("Light Diffuse", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit3("Light Specular", &mDirectionalLight.specular.r);
	}

	ImGui::Separator();
	
	mStandardEffect.DebugUI();
	mToonEffect.DebugUI();
	mShadowEffect.DebugUI();

	ImGui::Separator();

	ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);
	ImGui::Checkbox("Use ToonEffect", &mUseToon);

	ImGui::Separator();
	const auto& animClip = mMawMonsterAnimator.GetAnimClip(mMawMonsterAnimator.GetAnimClip());
	ImGui::DragFloat("Animation Playback", &mMawMonsterAnimator.mAnimationTick, 1.0f, 0.0f, animClip.tickDuration);
	if (ImGui::Button("Pause Animaton"))
		mPause = !mPause;
	if (ImGui::Button("Next Animation"))
	{
		animationIndex++;
		if (animationIndex >= animSize)
			animationIndex = 1;
		mMawMonsterAnimator.PlayAnimation(animationIndex, true);
	}
	if (ImGui::Button("Previous Animation"))
	{
		animationIndex--;
		if (animationIndex <= 0)
			animationIndex = 1;
		mMawMonsterAnimator.PlayAnimation(animationIndex, true);
	}

	ImGui::Text("Clip Playing: %d", mMawMonsterAnimator.GetAnimClip());
	ImGui::End();
}
