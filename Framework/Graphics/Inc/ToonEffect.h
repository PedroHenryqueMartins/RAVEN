#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "OutlineEffect.h"
#include "PixelShader.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace RAVEN::Graphics
{
	class ToonEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetCamera(const Camera* camera) { mCamera = camera; }
		void SetLightCamera(const Camera* camera) { mLightCamera = camera; }
		void SetDirectionalLight(const DirectionalLight* light) { mDirectionalLight = light; }
	private:
		struct TransformData
		{
			Math::Matrix4 world;
			Math::Matrix4 wvp[2];
			Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		struct SettingsData
		{
			int numSegments = 3;
			int useSkinning = 0;
			float padding[2] = { 0.0f };
		};

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		Sampler mSampler;

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = ConstantBuffer;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		LightBuffer mLightBuffer;
		SettingsBuffer mSettingsBuffer;
		SettingsData mSettingsData;

		int mValue = 3;
	};
}