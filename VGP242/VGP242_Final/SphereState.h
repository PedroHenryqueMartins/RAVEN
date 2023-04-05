#pragma once

#include <RAVEN/Inc/RAVEN.h>

namespace RAVEN
{

	class SphereState : public AppState
	{
	public:

		void Initialize() override;
		void Termiante() override;

		void Update(float deltaTime) override;
		void Render() override;


	private:

		struct TransformData
		{
			RAVEN::Math::Matrix4 worldMatrix;
			RAVEN::Math::Matrix4 viewMatrix;
			RAVEN::Math::Matrix4 projMatrix;
		};

		RAVEN::Graphics::Camera mCamera;

		RAVEN::Graphics::MeshPC mSphere;

		RAVEN::Graphics::MeshBuffer mMeshBuffer;
		RAVEN::Graphics::VertexShader mVertexShader;
		RAVEN::Graphics::PixelShader mPixelShader;


		using TransformBuffer = RAVEN::Graphics::TypedConstantBuffer<TransformData>; //making sure that TransformData is 16 bytes aligned.
		TransformBuffer mTransformBuffer;

		float mRotation = 0.0f;
	};
}

