#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "GroundPlaneComponent.h"
#include "ModelComponent.h"
#include "TerrainService.h"
#include "TransformComponent.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

void RenderService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();
	mTerrainService = GetWorld().GetService<TerrainService>();

	mDirectionalLight.ambient = Colors::White;
	mDirectionalLight.diffuse = Colors::White;
	mDirectionalLight.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });

	mStandardEffect.Initialize();
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.SetDepthBias(0.00001f);

	mTerrainEffect.Initialize();
	mTerrainEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mTerrainEffect.UseShadowMap() = true;
	mTerrainEffect.DepthBias() = 0.00005f;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(&mDirectionalLight);
}

void RenderService::Terminate()
{
	mShadowEffect.Terminate();
	mTerrainEffect.Terminate();
	mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
	//	Set Camera
	const auto& camera = mCameraService->GetCurrentCamera();
	mStandardEffect.SetCamera(&camera);
	mTerrainEffect.SetCamera(camera);

	//	Apply Transform
	for (auto& entry : mRenderEntries)
	{
		const Transform& transform = *entry.transformComponent;
		for (auto& renderObject : entry.renderGroup)
		{
			renderObject.transform = transform;
		}
	}

	//	Update ShadowMap
	mShadowEffect.Begin();
	{
		for (auto& entry : mRenderEntries)
			DrawRenderGroup(mShadowEffect, entry.renderGroup);
		if (mTerrainService)
			mShadowEffect.Render(mTerrainService->GetTerrainRenderObject());
	}
	mShadowEffect.End();

	//	Render Object
	mStandardEffect.Begin();
	{
		for (auto& entry : mRenderEntries)
			DrawRenderGroup(mStandardEffect, entry.renderGroup);
		for (auto& gameObj : GetWorld().GetUpdateList())
		{
			if (strcmp(gameObj->GetName().c_str(), "Ground"))
			{
				mStandardEffect.Render(GetWorld().GetGameObject("Ground")->GetComponent<GroundPlaneComponent>()->GetGroundRenderObject());
			}
		}
	}
	mStandardEffect.End();

	if (mTerrainService)
	{
		mTerrainEffect.Begin();
		mTerrainEffect.Render(mTerrainService->GetTerrainRenderObject());
		mTerrainEffect.End();
	}

	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(camera);
}

void RenderService::DebugUI()
{
	ImGui::Begin("Render Service", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("FPS: %f", mFPS);

	ImGui::Separator();

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
	mShadowEffect.DebugUI();

	ImGui::End();
}

void RenderService::SetShadowFocus(const Math::Vector3& focus)
{
	mShadowEffect.SetFocucs(focus);
}

void RenderService::Register(const ModelComponent* modelComponent)
{
	//	C++17 way, gets an empty reference to it.
	Entry& entry = mRenderEntries.emplace_back();

	auto& gameObject = modelComponent->GetOwner();
	entry.animatorComponent = gameObject.GetComponent<AnimatorComponent>();
	entry.modelComponent = modelComponent;
	entry.transformComponent = gameObject.GetComponent<TransformComponent>();
	const Animator* animator = nullptr;
	if (entry.animatorComponent)
		animator = &entry.animatorComponent->GetAnimator();
	entry.renderGroup = CreateRenderGroup(modelComponent->GetModel(), animator);
}

void RenderService::Unregister(const ModelComponent* modelComponent)
{

	//	The system is built in a way where the pointer is the ID. So no need to worry about dangling pointers, however that is not always correct.

	auto iter = std::find_if(mRenderEntries.begin(), mRenderEntries.end(), [&](const Entry& entry)
		{
			return entry.modelComponent == modelComponent;
		});

	if (iter != mRenderEntries.end())
	{
		Entry& entry = *iter;
		CleanUpRenderGroup(entry.renderGroup);
		mRenderEntries.erase(iter);
	}
}