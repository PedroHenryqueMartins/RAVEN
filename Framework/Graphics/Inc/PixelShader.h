#pragma once

namespace RAVEN::Graphics
{
	class PixelShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath, const char* entryPoint = "PS");
		// to use path as std::string, you do:
		//		filePath.u8string().c_str()
		// to use path as std::wstring, you do:
		//		filePath.c_str()
		void Terminate();

		void Bind() const;

	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
}