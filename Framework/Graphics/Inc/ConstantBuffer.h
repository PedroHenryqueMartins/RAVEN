#pragma once

namespace RAVEN::Graphics
{
	class ConstantBuffer
	{
	public:
		ConstantBuffer() = default;
		virtual ~ConstantBuffer();

		void Initialize(uint32_t bufferSize);
		void Terminate();

		void Update(const void* data) const;

		void BindVS(uint32_t slot = 0) const;	//	Binding Vertex Shader
		void BindPS(uint32_t slot = 0) const;	//	Binding Pixel Shader

	private:
		ID3D11Buffer* mBuffer = nullptr;
	};

	// Constant Buffer Memory
	// Everything needs to be 16 byte aligned
	//
	//	[float][float][float][float] <- 4 * 4 = 16 bytes per row



	template <class T>
	class TypedConstantBuffer final : public ConstantBuffer
	{
	public:
		using DataType = T;

		void Initialize()
		{
			static_assert(sizeof(DataType) % 16 == 0, "Data type must be 16 byte aligned.");
			ConstantBuffer::Initialize(sizeof(DataType));
		}

		void Update(const DataType& data) const
		{
			ConstantBuffer::Update(&data);
		}
	};
}
