#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;


void RAVEN::GameState::Initialize()
{
	// NDC - Normalized Device Coordinate
	//
	//          +-----------------+
	//         /                 /|
	//   1.0  +-----------------+ |
	//	      |        ^        | |
	//	      |        |        | |
	//	      | <------+------> | |
	//	      |        |        | | 1.0
	//	      |        v        |/
	//	-1.0  +-----------------+ 0.0
	//      -1.0               1.0
	//
	mVertices.push_back({ Vector3{  0.0f,  0.5f, 0.0f } });
	mVertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } });
	mVertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } });
	


	mMeshBuffer.Initialize(mVertices.data(), sizeof(Vertex), (uint32_t)mVertices.size());

	auto device = Graphics::GraphicsSystem::Get()->GetDevice();


	// Compile and create vertex shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoNothing.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");


	// Define vertex element descriptions
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	// Create the input layout
	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	// After all that, compile and create the Pixel Shader

	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoNothing.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
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

void RAVEN::GameState::Termiante()
{
	SafeRelease(mPixelShader);
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);


	mMeshBuffer.Terminate();
	
}

void RAVEN::GameState::Update(float deltaTime)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		MainApp().Quit();
	}
}

void RAVEN::GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	mMeshBuffer.Render();

}
