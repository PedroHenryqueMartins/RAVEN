#include "Precompiled.h"

#include "AnimationUtil.h"
#include "ShadowEffect.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);

	constexpr uint32_t depthMapSize = 4096;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, Texture::Format::RGBA_U32);

	mVertexShader.Initialize("../../Assets/Shaders/Shadow.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Shadow.fx");

	mTransformBuffer.Initialize();
	mBoneTransformBuffer.Initialize(256 * sizeof(Math::Matrix4));
	mSettingsBuffer.Initialize();
}

void ShadowEffect::Terminate()
{
	mSettingsBuffer.Terminate();
	mBoneTransformBuffer.Terminate();
	mTransformBuffer.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mDepthMapRenderTarget.Terminate();
}

void ShadowEffect::Begin()
{
	const auto direction = mDirectionalLight->direction;
	mLightCamera.SetDirection(direction);
	mLightCamera.SetPosition(mFocusPosition - (direction * 1000.0f));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(1500.0f);
	mLightCamera.SetSize(mSize, mSize);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mBoneTransformBuffer.BindVS(1);
	mSettingsBuffer.BindPS(2);
	mSettingsBuffer.BindVS(2);

	mDepthMapRenderTarget.BeginRender();
}

void ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::Render(const RenderObject& renderObject)
{

	Math::Matrix4 world = renderObject.transform.GetMatrix4();

	Math::Matrix4 view = mLightCamera.GetViewMatrix();
	Math::Matrix4 proj = mLightCamera.GetProjectionMatrix();

	TransformData transformData;
	transformData.wvp = Math::Transpose(world * view * proj);
	mTransformBuffer.Update(transformData);

	if (renderObject.skeleton != nullptr)
	{
		AnimationUtil::BoneTransforms boneTransforms;

		if (renderObject.animator != nullptr)
		{
			auto getBoneTransform = [&](const Bone* bone) { return renderObject.animator->GetToParentTransform(bone); };
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

	SettingsData settings;
	settings.useSkinning = (renderObject.skeleton != nullptr);
	mSettingsBuffer.Update(settings);
	renderObject.meshBuffer.Render();
}

void ShadowEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Shadow Effect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{ 150.0f, 150.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
}
