#include "Precompiled.h"

#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "TexturingEffect.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void TexturingEffect::Initialize()
{
	mVertexShader.Initialize("../../Assets/Shaders/Texturing.fx", Graphics::VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Texturing.fx");
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTransformBuffer.Initialize();
}

void TexturingEffect::Terminate()
{
	
	mTransformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void TexturingEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect -- no camera set!");

	
	mPixelShader.Bind();
	mVertexShader.Bind();

	mTransformBuffer.BindVS(0);
	mSampler.BindPS();

}

void TexturingEffect::End()
{
	BlendState::ClearState();
	DepthStencilState::ClearState();
}

void TexturingEffect::Render(const RenderObject& renderObject)
{
	Math::Matrix4 viewMatrix = mCamera->GetViewMatrix();
	Math::Matrix4 projMatrix = mCamera->GetProjectionMatrix();
	Math::Matrix4 worldMatrix = renderObject.transform.GetMatrix4();

	TransformData transformData;
	transformData.wvp = Transpose(worldMatrix * viewMatrix * projMatrix);
	mTransformBuffer.Update(transformData);

	BlendStateManager::Get()->Set(renderObject.blendMode);
	DepthStencilStateManager::Get()->Set(renderObject.depthTest, renderObject.depthWrite);

	auto textureManager = TextureManager::Get();
	textureManager->BindPS(renderObject.diffuseMapID, 0);
	//renderObject.diffuseMap.BindPS(0);
	renderObject.meshBuffer.Render();
}

void TexturingEffect::DebugUI()
{
	
}
