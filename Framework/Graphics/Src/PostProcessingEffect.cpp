#include "Precompiled.h"

#include "PostProcessingEffect.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void PostProcessingEffect::Initialize()
{
	mVertexShader.Initialize(L"../../Assets/Shaders/PostProcessing.fx", Graphics::VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/PostProcessing.fx");

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);

	mSettingsBuffer.Initialize();
}

void PostProcessingEffect::Terminate()
{

	mSettingsBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void PostProcessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	for (size_t i = 0; i < std::size(mTexture);++i)
	{
		if(mTexture[i])
			mTexture[i]->BindPS(i);
	}

	SettingsData settings;
	settings.mode = static_cast<int>(mMode);
	settings.timer = mTimer;
	settings.scale = mScale;
	settings.blursize = mBlursize;
	mSettingsBuffer.Update(settings);
	mSettingsBuffer.BindPS(0);


}

void PostProcessingEffect::End()
{
	for (size_t i = 0; i < std::size(mTexture); ++i)
	{
		Texture::UnbindPS(i);

	}
}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Post Processing Effect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentItem = static_cast<int>(mMode);
		const char* items[] =
		{
			"Default",
			"GrayScale",
			"Invert",
			"NightVision",
			"Pixel",
			"Blur",
			"Bloom",
			"Edge Detection"
		};

		if (ImGui::Combo("Mode", &currentItem, items, static_cast<int>(std::size(items))))
		{
			if (currentItem != static_cast<int>(mMode))
			{
				mMode = static_cast<Mode>(currentItem);

			}

		}

		if (mMode == Mode::Pixel)
		{
			ImGui::DragFloat("Pixalation Scale", &mScale, 0.001f, 0.0f, 1.0f);
		}

		if (mMode == Mode::Blur)
		{
			ImGui::DragFloat("Blur Size", &mBlursize, 0.1f, 0.0f, 50.0f);
		}
		
	}
}

void PostProcessingEffect::SetTexture(const Texture* texture, int index)
{
	mTexture[index] = texture;
}
