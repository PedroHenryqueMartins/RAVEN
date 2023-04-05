#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace RAVEN::Graphics
{
	class Camera;
	class RenderObject;
	class Terrain;
	class Texture;

	class TerrainEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera* camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture* shadowMap);

		bool& UseShadowMap() { return mUseShadowMap; }
		float& DepthBias() { return mDepthBias; }

	private:
		struct TransformData
		{
			Math::Matrix4 world;
			Math::Matrix4 wvp[2];
			Math::Vector3 viewPosition;
			float padding;
		};

		struct SettingsData
		{
			int useShadowMap = 0;
			float depthBias = 0.0f;
			float padding[2] = {};
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;

		Sampler mSampler;

		bool mUseShadowMap = false;
		float mDepthBias = 0.0f;
	};
}