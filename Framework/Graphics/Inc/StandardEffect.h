#pragma once


#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "RenderObject.h"
#include "Sampler.h"
#include "VertexShader.h"


namespace RAVEN::Graphics
{
	class StandardEffect
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
		void SetShadowMap(const Texture* shadowMap) { mShadowMap = shadowMap; }

		void SetDepthBias(float bias) { mSettingsData.depthBias = bias; }

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
			int useDiffuseMap = 1;
			int useSpecularMap = 1;
			int useNormalMap = 1;
			float displacementScale = 0.253f;

			int useShadowMap = 1;
			int useSkinning = 0;
			float depthBias = 0.0f;
			float padding = 0.0f;
		};

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		using TransformBuffer = TypedConstantBuffer<TransformData>; //making sure that TransformData is 16 bytes aligned.
		using BoneTransformBuffer = ConstantBuffer;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		TransformBuffer mTransformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		SettingsData mSettingsData;
	};
}