#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "RenderObject.h"
#include "Sampler.h"
#include "Texture.h"
#include "VertexShader.h"


namespace RAVEN::Graphics
{
	class OutlineEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		void SetTexture(const Texture* texture, int index = 0);
	private:

		struct SettingsData
		{
			float lineThickness = 1.0f;
			float padding[3] = { 0.0f };
		};

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const Texture* mTexture[2] = {};
		Sampler mSampler;

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;
		SettingsData mSettingsData;
		float mValue = 0.5f;
	};
}