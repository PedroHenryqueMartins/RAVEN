#include "Precompiled.h"
#include "PixelShader.h"

#include "GraphicsSystem.h"

void RAVEN::Graphics::PixelShader::Initialize(const std::filesystem::path& filePath, const char* entryPoint)
{
	auto device = GraphicsSystem::Get()->GetDevice();

	// After all that, compile and create the Pixel Shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		filePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void RAVEN::Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void RAVEN::Graphics::PixelShader::Bind() const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}
