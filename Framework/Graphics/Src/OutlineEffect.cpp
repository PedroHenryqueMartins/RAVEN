#include "Precompiled.h"

#include "OutlineEffect.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void OutlineEffect::Initialize()
{
	mVertexShader.Initialize(L"../../Assets/Shaders/Outline.fx", Graphics::VertexPX::Format);
	mPixelShader.Initialize(L"../../Assets/Shaders/Outline.fx");

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mSettingsBuffer.Initialize();
}

void OutlineEffect::Terminate()
{
	mSettingsBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void OutlineEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	mSettingsBuffer.BindVS(1);
	mSettingsBuffer.BindPS(1);

	for (size_t i = 0; i < std::size(mTexture);++i)
	{
		if(mTexture[i])
			mTexture[i]->BindPS(i);
	}

	SettingsData settings;
	settings.lineThickness = mValue;
	mSettingsBuffer.Update(settings);
}

void OutlineEffect::End()
{
	for (size_t i = 0; i < std::size(mTexture); ++i)
	{
		Texture::UnbindPS(i);
	}
}

void OutlineEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void OutlineEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Outline Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Line Thickness", &mValue, 0.01f, 0.0f, 2.0f);
	}
}

void OutlineEffect::SetTexture(const Texture* texture, int index)
{
	mTexture[index] = texture;
}