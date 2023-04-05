#include "Precompiled.h"

#include "AnimationUtil.h"
#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "GraphicsSystem.h"
#include "MeshBuilder.h"
#include "ToonEffect.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void ToonEffect::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/ToonShader.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/ToonShader.fx");
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize(256 * sizeof(Math::Matrix4));

	mLightBuffer.Initialize();
	mSettingsBuffer.Initialize();
}

void ToonEffect::Terminate()
{
	mSettingsBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ToonEffect::Begin()
{
	ASSERT(mCamera != nullptr, "ToonEffect -- no camera set!");
	ASSERT(mDirectionalLight != nullptr, "ToonEffect -- no light set!");
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);
	mBoneTransformBuffer.BindVS(1);
	mLightBuffer.BindPS(2);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);
	mSampler.BindPS();
	mLightBuffer.Update(*mDirectionalLight);

	SettingsData settings;
	settings.numSegments = mValue;
	mSettingsBuffer.Update(settings);
}

void ToonEffect::End()
{
	BlendState::ClearState();
	DepthStencilState::ClearState();
}

void ToonEffect::Render(const RenderObject& renderObject)
{
	Math::Matrix4 worldMatrix = renderObject.transform.GetMatrix4();
	Math::Matrix4 viewMatrix = mCamera->GetViewMatrix();
	Math::Matrix4 projMatrix = mCamera->GetProjectionMatrix();

	TransformData transformData;
	transformData.viewPosition = mCamera->GetPosition();

	transformData.world = Transpose(worldMatrix);
	transformData.wvp[0] = Transpose(worldMatrix * viewMatrix * projMatrix);

	if (mLightCamera != nullptr)
	{
		Math::Matrix4 lightViewMatrix = mLightCamera->GetViewMatrix();
		Math::Matrix4 lightProjMatrix = mLightCamera->GetProjectionMatrix();
		transformData.wvp[1] = Transpose(worldMatrix * lightViewMatrix * lightProjMatrix);
	}

	mTransformBuffer.Update(transformData);

	if (renderObject.skeleton != nullptr)
	{
		AnimationUtil::BoneTransforms boneTransforms;

		if (renderObject.animator != nullptr)
		{
			auto getBoneTransform = [&](const Bone* bone) {return renderObject.animator->GetToParentTransform(bone); };
			AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, getBoneTransform, boneTransforms);
		}
		else
		{
			auto getBoneTransform = [](const Bone* bone) { return bone->toParentTransform; };
			AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, getBoneTransform, boneTransforms);
		} 
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		for (auto& transform : boneTransforms)
		{
			transform = Math::Transpose(transform);
		}
		boneTransforms.resize(256);
		mBoneTransformBuffer.Update(boneTransforms.data());
	}

	BlendStateManager::Get()->Set(renderObject.blendMode);
	DepthStencilStateManager::Get()->Set(renderObject.depthTest, renderObject.depthWrite);

	auto textureManager = TextureManager::Get();
	textureManager->BindPS(renderObject.diffuseMapID, 0);
	
	SettingsData settings;
	settings.useSkinning = (renderObject.skeleton != nullptr);
	mSettingsBuffer.Update(settings);
	//renderObject.diffuseMap.BindPS(0);
	
	renderObject.meshBuffer.Render();
}

void ToonEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Toon Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("Number of Segments", &mValue, 1, 2, 4);

	}
}