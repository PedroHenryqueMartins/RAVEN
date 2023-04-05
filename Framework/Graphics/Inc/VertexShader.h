#pragma once

namespace RAVEN::Graphics
{
	class VertexShader
	{
	public:
		template <class VertexType>
		void Initialize(const std::filesystem::path& filePath)
		{
			Initialize(filePath, VertexType::Format);
		}

		void Initialize(const std::filesystem::path& filePath, uint32_t vertexFormat);
		// to use path as std::string, you do:
		//		filePath.u8string().c_str()
		// to use path as std::wstring, you do:
		//		filePath.c_str()
		void Terminate();

		void Bind() const;

	private:
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;
	};
}