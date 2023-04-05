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

		std::vector<RAVEN::Graphics::VertexPC> mVertices;

		RAVEN::Graphics::MeshBuffer mMeshBuffer;
		RAVEN::Graphics::VertexShader mVertexShader;
		RAVEN::Graphics::PixelShader mPixelShader;

	};
}

