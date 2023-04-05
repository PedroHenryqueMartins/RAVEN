#include "Precompiled.h"
#include "GaussianBlurEffect.h"

#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void GaussianBlurEffect::Initialize()
{
	auto graphicsSystem = GraphicsSystem::Get();
	const auto screenWidth = graphicsSystem->GetBackBufferWidth();
	const auto screenHeight = graphicsSystem->GetBackBufferHeight();
	mHorizontalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mVerticalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	mVertexShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", Graphics::VertexPX::Format);
	mHorizontalBlurPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "HorizontalBlurPS");
	mVerticalBlurPixelShader.Initialize(L"../../Assets/Shaders/GaussianBlur.fx", "VerticalBlurPS");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	mSettingsBuffer.Initialize();
}

void GaussianBlurEffect::Terminate()
{
	mSettingsBuffer.Terminate();

	mSampler.Terminate();

	mVerticalBlurPixelShader.Terminate();
	mHorizontalBlurPixelShader.Terminate();
	mVertexShader.Terminate();

	mVerticalBlurRenderTarget.Terminate();
	mHorizontalBlurRenderTarget.Terminate();
}

void GaussianBlurEffect::Begin()
{
	mVertexShader.Bind();

	auto graphicsSystem = GraphicsSystem::Get();
	const auto screenWidth = graphicsSystem->GetBackBufferWidth();
	const auto screenHeight = graphicsSystem->GetBackBufferHeight();

	SettingsData settingsData;
	settingsData.screenWidth = static_cast<float>(screenWidth);
	settingsData.screenHeight = static_cast<float>(screenHeight);
	settingsData.multiplier = mBlurSaturation;
	mSettingsBuffer.Update(settingsData);
	mSettingsBuffer.BindPS(0);

	mSampler.BindPS(0);
}

void GaussianBlurEffect::End()
{
}

void GaussianBlurEffect::Render(const RenderObject& renderObject)
{
	auto graphicsSystem = GraphicsSystem::Get();

	// Horizontal blur
	mHorizontalBlurRenderTarget.BeginRender();
		mSourceTexture->BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
	mHorizontalBlurRenderTarget.EndRender();

	for (int i = 1; i < mBlurIterations; ++i)
	{
		// Vertical blur
		mVerticalBlurRenderTarget.BeginRender();
		mHorizontalBlurRenderTarget.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mVerticalBlurRenderTarget.EndRender();

		// Horizontal blur
		mHorizontalBlurRenderTarget.BeginRender();
		mVerticalBlurRenderTarget.BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mHorizontalBlurRenderTarget.EndRender();
	}

	// Vertical blur
	mVerticalBlurRenderTarget.BeginRender();
		mHorizontalBlurRenderTarget.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
	mVerticalBlurRenderTarget.EndRender();
}

void GaussianBlurEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Gaussian Blur Effect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("Blur Iterations", &mBlurIterations, 1, 1, 100);
		ImGui::DragFloat("Blur Saturation", &mBlurSaturation, 0.001f, 1.0f, 100.0f);
	}
}
