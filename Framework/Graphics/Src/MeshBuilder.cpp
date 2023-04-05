#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;


//	Corrected Mesh Build for Cylinder and Sphere!

MeshPC MeshBuilder::CreateCubePC()
{
	MeshPC mesh;

	mesh.vertices.push_back({ Vector3{  0.5f,  -0.5f,  0.5f }, Colors::Red }); //0
	mesh.vertices.push_back({ Vector3{  0.5f,  -0.5f, -0.5f }, Colors::Orange }); //1
	mesh.vertices.push_back({ Vector3{  0.5f,   0.5f, -0.5f }, Colors::Yellow }); //2
	mesh.vertices.push_back({ Vector3{  0.5f,   0.5f,  0.5f }, Colors::Green }); //3
	mesh.vertices.push_back({ Vector3{ -0.5f,  -0.5f,  0.5f }, Colors::Blue }); //4
	mesh.vertices.push_back({ Vector3{ -0.5f,  -0.5f, -0.5f }, Colors::Indigo }); //5
	mesh.vertices.push_back({ Vector3{ -0.5f,   0.5f, -0.5f }, Colors::Purple }); //6
	mesh.vertices.push_back({ Vector3{ -0.5f,   0.5f,  0.5f }, Colors::White }); //7

	mesh.indices =
	{
		0, 1, 2, 0, 2, 3,
		3, 2, 6, 3, 6, 7,
		7, 6, 5, 7, 5, 4,
		4, 5, 1, 4, 1, 0,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};

	return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(float size, int rows, int columns)
{
	MeshPC mesh;

	const float cellWidth = size / (columns - 1);
	const float cellHeight = size / (rows - 1);
	const float xOffset = size * -0.5f;
	const float yOffset = size * -0.5f;

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < columns; ++c)
		{
			const float x = (c * cellWidth) + xOffset;
			const float y = 0.0f;
			const float z = (r * cellHeight) + yOffset;
			const Color color =
				c % 2 == 0
				? (r % 2 == 0 ? Colors::White : Colors::Red)
				: (r % 2 == 0 ? Colors::Orange : Colors::White);
			mesh.vertices.push_back({ {x, y, z}, color });
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(float height, float radius, int rings, int slices)
{
	MeshPC mesh;

	const int rows = rings + 2;
	const int columns = slices + 1;

	const float yStep = height / (rings - 1);
	const float yOffset = height * -0.5f;

	const float thetaStep = Constants::TwoPi / slices;

	for (int r = 0; r < rows; ++r)
	{
		float y = ((r - 1) * yStep) + yOffset;
		float ringRadius = radius;

		if (r == 0)
		{
			y = yOffset;
			ringRadius = 0.0f;
		}
		else if (r + 1 == rows)
		{
			y = height + yOffset;
			ringRadius = 0.0f;
		}

		for (int s = 0; s < columns; ++s)
		{
			const float theta = s * thetaStep;
			const float x = -sin(theta) * ringRadius;
			const float z = cos(theta) * ringRadius;
			const Color color =
				s % 2 == 0
				? (r % 2 == 0 ? Colors::Blue : Colors::Blue)
				: (r % 2 == 0 ? Colors::Blue : Colors::Blue);
			mesh.vertices.push_back({ {x, y, z}, color });
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(float radius, int rings, int slices)
{
	MeshPC mesh;

	const int rows = rings;
	const int columns = slices + 1;

	const float phiStep = Constants::Pi / (rings - 1);
	const float thetaStep = Constants::TwoPi / slices;

	for (int r = 0; r < rows; ++r)
	{
		const float phi = r * phiStep;
		float y = -cos(phi) * radius;
		float ringRadius = sin(phi) * radius;

		for (int s = 0; s < columns; ++s)
		{
			const float theta = s * thetaStep;
			const float x = -sin(theta) * ringRadius;
			const float z = cos(theta) * ringRadius;
			const Color color =
				s % 2 == 0
				? (r % 2 == 0 ? Colors::Purple : Colors::Blue)
				: (r % 2 == 0 ? Colors::Blue: Colors::Blue);
			mesh.vertices.push_back({ {x, y, z}, color });
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int rings, int slices)
{

	MeshPX mesh;

	const int rows = rings;
	const int columns = slices + 1;

	const float phiStep = Constants::Pi / (rings - 1);
	const float thetaStep = Constants::TwoPi / slices;

	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (int r = 0; r < rows; ++r)
	{
		const float phi = r * phiStep;
		float y = -cos(phi) * radius;
		float ringRadius = sin(phi) * radius;

		for (int s = 0; s < columns; ++s)
		{
			const float theta = s * thetaStep;
			const float x = -sin(theta) * ringRadius;
			const float z = cos(theta) * ringRadius;
			const float u = s * uStep;
			const float v = 1.0f - r * vStep;
			mesh.vertices.push_back({ {x, y, z}, {u, v } });
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPX MeshBuilder::CreateSkybox(float radius, int rings, int slices)
{
	MeshPX mesh;

	const int rows = rings;
	const int columns = slices + 1;

	const float phiStep = Constants::Pi / (rings - 1);
	const float thetaStep = Constants::TwoPi / slices;

	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (int r = 0; r < rows; ++r)
	{
		const float phi = r * phiStep;
		float y = -cos(phi) * radius;
		float ringRadius = sin(phi) * radius;

		for (int s = 0; s < columns; ++s)
		{
			const float theta = s * thetaStep;
			const float x = -sin(theta) * ringRadius;
			const float z = cos(theta) * ringRadius;
			const float u = s * uStep;
			const float v = 1.0f - r * vStep;
			mesh.vertices.push_back({ {x, y, z}, {u, v } });
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
		}
	}

	return mesh;
}

Mesh MeshBuilder::CreateSphere(float radius, int rings, int slices)
{
	Mesh mesh;

	const int rows = rings;
	const int columns = slices + 1;

	const float phiStep = Constants::Pi / (rings - 1);
	const float thetaStep = Constants::TwoPi / slices;

	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (int r = 0; r < rows; ++r)
	{
		const float phi = r * phiStep;
		float y = -cos(phi) * radius;
		float ringRadius = sin(phi) * radius;

		for (int s = 0; s < columns; ++s)
		{
			const float theta = s * thetaStep;
			const float x = -sin(theta) * ringRadius;
			const float z = cos(theta) * ringRadius;
			const float u = s * uStep;
			const float v = 1.0f - r * vStep;
			const Vector3 position{ x, y, z };
			const Vector3 normal = Normalize(position);
			const Vector3 tanget = Normalize({ -normal.z, 0.0f, normal.x });
			const Vector2 texCoord{ u, v };
			mesh.vertices.push_back({
							position,
							normal,
							tanget,
							texCoord });
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}

MeshPX MeshBuilder::CreateNDCQuad()
{
	MeshPX mesh;
	

	mesh.vertices = {
		{ {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
		{ {-1.0f, +1.0f, 0.0f}, {0.0f, 0.0f} },
		{ {+1.0f, +1.0f, 0.0f}, {1.0f, 0.0f} },
		{ {+1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
	}; 

	mesh.indices = {
		0, 1, 2, 0, 2, 3
	};

	return mesh;
}

Mesh MeshBuilder::CreatePlane(float size, int rows, int columns, float tilling)
{
	Mesh mesh;

	const float cellWidth = size / (columns - 1);
	const float cellHeight = size / (rows - 1);
	const float xOffset = size * -0.5f;
	const float zOffset = size * -0.5f;

	const float uStep = tilling / columns;
	const float vStep = tilling / (rows - 1);

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < columns; ++c)
		{
			const float x = (c * cellWidth) + xOffset;
			const float y = 0.0f;
			const float z = (r * cellHeight) + zOffset;
			const float u = uStep * c;
			const float v = vStep * r;
			mesh.vertices.push_back({ 
				{x, y, z}, 
				Vector3::YAxis, 
				Vector3::XAxis,
				{u, v}
				});
		}
	}

	for (int r = 0; r + 1 < rows; ++r)
	{
		for (int c = 0; c + 1 < columns; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));

			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}