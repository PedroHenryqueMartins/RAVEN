#include "Precompiled.h"

#include "AnimationUtil.h"
#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "StandardEffect.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void StandardEffect::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize(256 * sizeof(Math::Matrix4));
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
}

void StandardEffect::Terminate()
{
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect -- no camera set!");
	ASSERT(mDirectionalLight != nullptr, "StandardEffect -- no light set!");
	
	mPixelShader.Bind();
	mVertexShader.Bind();

	mTransformBuffer.BindVS(0);
	mBoneTransformBuffer.BindVS(1);
	mLightBuffer.BindPS(2);
	mMaterialBuffer.BindPS(3);
	mSettingsBuffer.BindVS(4);
	mSettingsBuffer.BindPS(4);	
	mSampler.BindPS();

	mLightBuffer.Update(*mDirectionalLight);
}

void StandardEffect::End()
{
	for (uint32_t i = 0; i < 5; ++i)
	{
		Texture::UnbindPS(i);
	}

	BlendState::ClearState();
	DepthStencilState::ClearState();
}

void StandardEffect::Render(const RenderObject& renderObject)
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
	mMaterialBuffer.Update(renderObject.material);

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
			auto getBoneTransform = [](const Bone* bone) {return bone->toParentTransform; };
			AnimationUtil::ComputeBoneTransforms(*renderObject.skeleton, getBoneTransform, boneTransforms);
		}
		AnimationUtil::ApplyBoneOffset(*renderObject.skeleton, boneTransforms);

		for (auto& transform : boneTransforms)
		{
			transform = Math::Transpose(transform);	
		}
		boneTransforms.resize(256); // We need to pad out the transform array as the constant buffer expects 256 matrices
		mBoneTransformBuffer.Update(boneTransforms.data());
	}

	BlendStateManager::Get()->Set(renderObject.blendMode); 
	DepthStencilStateManager::Get()->Set(renderObject.depthTest, renderObject.depthWrite);

	auto textureManager = TextureManager::Get();
	textureManager->BindPS(renderObject.diffuseMapID, 0);
	textureManager->BindPS(renderObject.specularMapID, 1);
	textureManager->BindVS(renderObject.displacementMapID, 2);
	textureManager->BindPS(renderObject.normalMapID, 3);

	SettingsData settings;
	settings.useDiffuseMap = mSettingsData.useDiffuseMap && renderObject.diffuseMapID != 0;
	settings.useSpecularMap = mSettingsData.useSpecularMap && renderObject.specularMapID != 0;
	settings.useNormalMap = mSettingsData.useNormalMap && renderObject.normalMapID != 0;
	settings.displacementScale = renderObject.displacementMapID != 0 ? mSettingsData.displacementScale : 0.0f;
	settings.useShadowMap = 0;
	settings.useSkinning = (renderObject.skeleton != nullptr);
	if (mShadowMap != nullptr && mSettingsData.useShadowMap)
	{
		settings.useShadowMap = 1;
		settings.depthBias = mSettingsData.depthBias;
		mShadowMap->BindPS(4);
	}
	mSettingsBuffer.Update(settings);

	renderObject.meshBuffer.Render();
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool checked = mSettingsData.useDiffuseMap == 1;
		if (ImGui::Checkbox("Use Diffuse Map", &checked))
		{
			mSettingsData.useDiffuseMap = checked ? 1 : 0;
		}

		checked = mSettingsData.useSpecularMap == 1;
		if (ImGui::Checkbox("Use Specular Map", &checked))
		{
			mSettingsData.useSpecularMap = checked ? 1 : 0;
		}

		checked = mSettingsData.useNormalMap == 1;
		if (ImGui::Checkbox("Use Normal Map", &checked))
		{
			mSettingsData.useNormalMap = checked ? 1 : 0;
		}

		ImGui::DragFloat("Displacement Scale", &mSettingsData.displacementScale, 0.1f, 0.0f, 1.0f);

		checked = mSettingsData.useShadowMap == 1;
		if (ImGui::Checkbox("Use Shadow Map", &checked))
		{
			mSettingsData.useShadowMap = checked ? 1 : 0;
		}

		ImGui::DragFloat("Depth Bias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
	}
}
