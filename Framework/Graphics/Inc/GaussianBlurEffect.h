#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace RAVEN::Graphics
{
	class GaussianBlurEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetSourceTexture(const Texture& texture) { mSourceTexture = &texture; }

		const Texture& GetHorizontalBlurTexture() const { return mHorizontalBlurRenderTarget; }
		const Texture& GetVerticalBlurTexture() const { return mVerticalBlurRenderTarget; }
		const Texture& GetResultTexture() const { return mVerticalBlurRenderTarget; }

		void SetBlurIterations(int iteration) { mBlurIterations = iteration; }
		void SetBlurSaturation(float saturation) { mBlurSaturation = saturation; }

	private:
		struct SettingsData
		{
			float screenWidth;
			float screenHeight;
			float multiplier;
			float padding;
		};

		RenderTarget mHorizontalBlurRenderTarget;
		RenderTarget mVerticalBlurRenderTarget;

		VertexShader mVertexShader;
		PixelShader mHorizontalBlurPixelShader;
		PixelShader mVerticalBlurPixelShader;

		Sampler mSampler;

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		const Texture* mSourceTexture = nullptr;

		int mBlurIterations = 1;
		float mBlurSaturation = 1.0f;
	};
}