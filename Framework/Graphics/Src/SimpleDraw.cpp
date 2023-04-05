#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	// Private implementation - a strong encapsulation technique to hide details from the user
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount)
		{
			mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
			mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
			mConstantBuffer.Initialize(sizeof(Math::Matrix4));
			mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount, true);

			mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mFaceVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mMaxVertexCount = maxVertexCount;
		}

		void Terminate()
		{
			mMeshBuffer.Terminate();
			mConstantBuffer.Terminate();
			mPixelShader.Terminate();
			mVertexShader.Terminate();
		}

		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color)
		{
			if (mLineVertexCount + 2 <= mMaxVertexCount)
			{
				mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
				mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
			}
		}

		void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, Color color)
		{
			if (mFaceVertexCount + 3 <= mMaxVertexCount)
			{
				mFaceVertices[mFaceVertexCount++] = VertexPC{ v0, color };
				mFaceVertices[mFaceVertexCount++] = VertexPC{ v1, color };
				mFaceVertices[mFaceVertexCount++] = VertexPC{ v2, color };
			}
		}

		void Render(const Camera& camera)
		{
			auto viewMatrix = camera.GetViewMatrix();
			auto projMatrix = camera.GetProjectionMatrix();

			mVertexShader.Bind();
			mPixelShader.Bind();

			Math::Matrix4 transform = Math::Transpose(viewMatrix * projMatrix);
			mConstantBuffer.Update(&transform);
			mConstantBuffer.BindVS(0);

			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
			mMeshBuffer.Render();

			mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
			mMeshBuffer.Update(mFaceVertices.get(), mFaceVertexCount);
			mMeshBuffer.Render();

			// Rewind buffer so we start collecting vertices from the beginning
			mLineVertexCount = 0;
			mFaceVertexCount = 0;
		}

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;

		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFaceVertices;
		uint32_t mLineVertexCount = 0;
		uint32_t mFaceVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, Color color)
{
	sInstance->AddFace(v0, v1, v2, color);
}

void SimpleDraw::Render(const Camera& camera)
{
	sInstance->Render(camera);
}


void SimpleDraw::AddAABB(const Math::Vector3& center, const Math::Vector3& extend, Color color, bool fill)
{
	const float minX = center.x - extend.x;
	const float minY = center.y - extend.y;
	const float minZ = center.z - extend.z;
	const float maxX = center.x + extend.x;
	const float maxY = center.y + extend.y;
	const float maxZ = center.z + extend.z;


	const Math::Vector3 NNN = { minX, minY, minZ };
	const Math::Vector3 NNP = { minX, minY, maxZ };
	const Math::Vector3 NPN = { minX, maxY, minZ };
	const Math::Vector3 NPP = { minX, maxY, maxZ };
	const Math::Vector3 PNN = { maxX, minY, minZ };
	const Math::Vector3 PNP = { maxX, minY, maxZ };
	const Math::Vector3 PPN = { maxX, maxY, minZ };
	const Math::Vector3 PPP = { maxX, maxY, maxZ };


	if (fill)
	{

		sInstance->AddFace(NNN, NPN, PPN, color);
		sInstance->AddFace(NNN, PPN, PNN, color);

		sInstance->AddFace(PNN, PPN, PPP, color);
		sInstance->AddFace(PNN, PPP, PNP, color);

		sInstance->AddFace(PNP, PPP, NPP, color);
		sInstance->AddFace(PNP, NPP, NNP, color);

		sInstance->AddFace(NNP, NPP, NPN, color);
		sInstance->AddFace(NNP, NPN, NNN, color);

		sInstance->AddFace(NPN, NPP, PPP, color);
		sInstance->AddFace(NPN, PPP, PPN, color);

		sInstance->AddFace(NNP, NNN, PNN, color);
		sInstance->AddFace(NNP, PNN, PNP, color);

	}
	else
	{
		sInstance->AddLine(NNN, NNP, color);
		sInstance->AddLine(NNP, PNP, color);
		sInstance->AddLine(PNP, PNN, color);
		sInstance->AddLine(PNN, NNN, color);

		sInstance->AddLine(NNN, NPN, color);
		sInstance->AddLine(NNP, NPP, color);
		sInstance->AddLine(PNP, PPP, color);
		sInstance->AddLine(PNN, PPN, color);

		sInstance->AddLine(NPN, NPP, color);
		sInstance->AddLine(NPP, PPP, color);
		sInstance->AddLine(PPP, PPN, color);
		sInstance->AddLine(PPN, NPN, color);
	}

	

}



void SimpleDraw::AddTransform(const Math::Matrix4& transform)
{
	Math::Vector3 right(transform._11, transform._12, transform._13);
	Math::Vector3 up(transform._21, transform._22, transform._23);
	Math::Vector3 look(transform._31, transform._32, transform._33);

	Math::Vector3 position = Math::Vector3::Zero;

	sInstance->AddLine(position, position + right, Colors::Red);
	sInstance->AddLine(position, position + up, Colors::Green);
	sInstance->AddLine(position, position + look, Colors::Blue);
	
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{

	Math::Vector3 firstVectorX{  -(size * .5f), 0.0f, -(size * .5f) };
	Math::Vector3 secondVectorX{   size * .5f,  0.0f, -(size * .5f) };
	Math::Vector3 firstVectorZ{  -(size * .5f), 0.0f, -(size * .5f) };
	Math::Vector3 secondVectorZ{ -(size * .5f), 0.0f, size * .5f };

	for (int i = 0; i <= size; ++i)
	{
		sInstance->AddLine(firstVectorX, secondVectorX, color);
		firstVectorX.z += 1.0f;
		secondVectorX.z += 1.0f;
	}

	for (int i = 0; i <= size; ++i)
	{
		sInstance->AddLine(firstVectorZ, secondVectorZ, color);
		firstVectorZ.x += 1.0f;
		secondVectorZ.x += 1.0f;

	}

}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, Color color, size_t rings, size_t slices)
{


	const int rows = rings;
	const int columns = slices + 1;

	const float phiStep = Math::Constants::Pi / (rings - 1);
	const float thetaStep = Math::Constants::TwoPi / slices;

	std::vector<Math::Vector3> sphereVertices;

	const int vertexCount = columns * rows;
	sphereVertices.reserve(vertexCount);

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < columns; ++c)
		{

			const float phi = r * phiStep;
			const float theta = c * thetaStep;
			float ringRadius = sin(phi) * radius;
			
			const float x = -sin(theta) * ringRadius;
			const float y = -cos(phi) * radius;
			const float z = cos(theta) * ringRadius;

			Math::Vector3 vertex = { x, y, z };

			sphereVertices.push_back(vertex + center);

		}
	}

	for (int r = 0; r < rows - 1; ++r)
	{
		for (int c = 0; c < columns - 1; ++c)
		{
			const float firstVertex = (c + 0) + ((r + 0) * columns);
			const float secondVertex = (c + 0) + ((r + 1) * columns);
			const float thirdVertex = (c + 1) + ((r + 0) * columns);

			sInstance->AddLine(sphereVertices[firstVertex], sphereVertices[secondVertex], color);
			sInstance->AddLine(sphereVertices[firstVertex], sphereVertices[thirdVertex], color);
		}
	}
}
