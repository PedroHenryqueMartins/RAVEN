#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "RenderObject.h"
#include "Sampler.h"
#include "Texture.h"
#include "VertexShader.h"


namespace RAVEN::Graphics
{
	class PostProcessingEffect
	{
	public:

		enum class Mode
		{
			Default,
			Grayscale,
			Invert,
			NightVision,
			Pixel,
			Blur,
			Bloom,
			EdgeDetection
		};

		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetTexture(const Texture* texture, int index = 0);
		void SetTimer(float timer) { mTimer = timer; }
		void SetMode(Mode mode) { mMode = mode; }

	private:

		struct SettingsData
		{
			int mode = 0;
			float timer = 0.0f;
			float scale = 0.0f;
			float blursize = 0.0f;
		};

		SettingsData mSettingsData;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const Texture* mTexture[2] = {};
		Sampler mSampler;

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		Mode mMode = Mode::Default;
		float mTimer = 0.0f;
		float mScale = 0.0f;
		float mBlursize = 8.0f;
	};
}