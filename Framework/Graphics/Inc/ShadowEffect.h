#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "PixelShader.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "VertexShader.h"

namespace RAVEN::Graphics
{
	class ShadowEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetDirectionalLight(const DirectionalLight* light) { mDirectionalLight = light; }

		void SetFocucs(const Math::Vector3& focusPosition) { mFocusPosition = focusPosition; }
		void SetSize(float size) { mSize = size; }

		const Camera& GetLightCamera() const { return mLightCamera; }
		const Texture& GetDepthMap() const { return mDepthMapRenderTarget; }

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		struct SettingsData
		{
			int useSkinning = 0;
			float padding[3]{ 0.0f };
		};

		Camera mLightCamera;

		const DirectionalLight* mDirectionalLight = nullptr;

		RenderTarget mDepthMapRenderTarget;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = ConstantBuffer;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		SettingsBuffer mSettingsBuffer;

		Math::Vector3 mFocusPosition = Math::Vector3::Zero;
		float mSize = 100.0f;
	};
}