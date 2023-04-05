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

		struct TransformData
		{
			RAVEN::Math::Matrix4 worldMatrix;
			RAVEN::Math::Matrix4 viewMatrix;
			RAVEN::Math::Matrix4 projMatrix;
		};

		RAVEN::Graphics::Camera mCamera;

		RAVEN::Graphics::MeshPC mCube;
		RAVEN::Graphics::MeshPC mPlane;
		RAVEN::Graphics::MeshPC mCylinder;
		RAVEN::Graphics::MeshPC mSphere;
		RAVEN::Graphics::MeshPX mSphere2;

		RAVEN::Graphics::MeshBuffer mCubeMeshBuffer;
		RAVEN::Graphics::MeshBuffer mPlaneMeshBuffer;
		RAVEN::Graphics::MeshBuffer mCylinderMeshBuffer;
		RAVEN::Graphics::MeshBuffer mSphereMeshBuffer;
		RAVEN::Graphics::MeshBuffer mSphereMeshBuffer2;


		RAVEN::Graphics::VertexShader mVertexShader;
		RAVEN::Graphics::PixelShader mPixelShader;

		RAVEN::Graphics::VertexShader mTexturingVertexShader;
		RAVEN::Graphics::PixelShader mTexturingPixelShader;

		RAVEN::Graphics::Texture mTexture;
		RAVEN::Graphics::Sampler mSampler;


		using TransformBuffer = RAVEN::Graphics::TypedConstantBuffer<TransformData>; //making sure that TransformData is 16 bytes aligned.
		TransformBuffer mTransformBuffer;

		RAVEN::Math::Vector3 mRotation = RAVEN::Math::Vector3::Zero;
		RAVEN::Math::Vector3 mPosition = RAVEN::Math::Vector3::Zero;
		RAVEN::Math::Vector3 mScale = RAVEN::Math::Vector3::One;
	};
}

