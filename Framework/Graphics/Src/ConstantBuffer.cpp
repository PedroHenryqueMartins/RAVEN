#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace RAVEN::Graphics;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mBuffer == nullptr, "ConstantBuffer -- Buffer not released!");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC bufferDesc{};	//	initialization set to zero
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &mBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer.");
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mBuffer);
}

void ConstantBuffer::Update(const void* data) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetConstantBuffers(slot, 1, &mBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetConstantBuffers(slot, 1, &mBuffer);
}