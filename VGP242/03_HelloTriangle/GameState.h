#pragma once

#include <RAVEN/Inc/RAVEN.h>

namespace RAVEN
{

	class GameState : public AppState
	{
	public:
		
		void Initialize() override;
		void Termiante() override;
		
		void Update(float deltaTime) override;
		void Render() override;


	private:

		struct Vertex
		{
			RAVEN::Math::Vector3 position;
		};

		std::vector<Vertex> mVertices;

		RAVEN::Graphics::MeshBuffer mMeshBuffer;

		//	TODO: Move these pointers into Graphics::VertexShader
		ID3D11VertexShader* mVertexShader = nullptr;
		ID3D11InputLayout* mInputLayout = nullptr;

		//	TODO: Move this into Graphics::PixelShader
		ID3D11PixelShader* mPixelShader = nullptr;

	};
}

// About GraphicsSystem:
// =====================
// This singleton primarily manages two Direct3D interface pointers, namely:
//
//	ID3D11Device* GetDevice();
//	ID3D11DeviceContext* GetContext();
//
// The device pointer represents the current configuration active for the GPU.
// This will be the pointer used for allocating graphics memory (VRAM).
//
// The context pointer is used to send commands to the GPU for rendering.
// Depending on which pipe stages you want to command, you will use the functions
// with the corresponding two-letter prefix (e.g. for Vertex Shader, you will use VS).
