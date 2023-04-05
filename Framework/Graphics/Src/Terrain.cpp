#include "Precompiled.h"
#include "Terrain.h"

#include "MeshUtil.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void Terrain::Initialize(std::filesystem::path fileName, float heightScale)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "rb");

	fseek(file, 0L, SEEK_END);
	const size_t fileSize = ftell(file);
	const size_t dimension = (size_t)sqrt(static_cast<float>(fileSize));
	fseek(file, 0L, SEEK_SET);

	mRows = dimension;
	mColumns = dimension;

	const float tileCount = 35;

	mesh.vertices.resize(mRows * mColumns);
	for (size_t z = 0; z < mRows; ++z)
	{
		for (size_t x = 0; x < mColumns; ++x)
		{
			const auto c = fgetc(file);
			const float height = c / 255.0f * heightScale;
			const size_t index = x + (z * mRows);

			auto& vertex = mesh.vertices[index];
			const float posX = static_cast<float>(x);
			const float posZ = static_cast<float>(z);
			vertex.position = Math::Vector3{ posX, height, posZ };
			vertex.texCoord.x = static_cast<float>(x) / mColumns * tileCount;
			vertex.texCoord.y = static_cast<float>(z) / mRows * tileCount;
		}
	}

	fclose(file);

	const size_t cells = (mRows - 1) * (mColumns - 1);
	mesh.indices.reserve(cells * 6);

	for (size_t z = 0; z < mRows - 1; ++z)
	{
		for (size_t x = 0; x < mColumns - 1; ++x)
		{
			const uint32_t bl = static_cast<uint32_t>((x + 0) + (z + 0) * mColumns);
			const uint32_t tl = static_cast<uint32_t>((x + 0) + (z + 1) * mColumns);
			const uint32_t br = static_cast<uint32_t>((x + 1) + (z + 0) * mColumns);
			const uint32_t tr = static_cast<uint32_t>((x + 1) + (z + 1) * mColumns);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tl);
			mesh.indices.push_back(tr);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tr);
			mesh.indices.push_back(br);
		}
	}

	MeshUtil::ComputeNormals(mesh);
}

float Graphics::Terrain::GetHeight(const Math::Vector3& position) const
{
	const int x = static_cast<int>(position.x);
	const int z = static_cast<int>(position.z);

	if (x < 0 || z < 0 || x + 1 >= mColumns || z + 1 >= mRows)
		return 0.0f;

	const uint32_t bl = static_cast<uint32_t>((x + 0) + (z + 0) * mColumns);
	const uint32_t tl = static_cast<uint32_t>((x + 0) + (z + 1) * mColumns);
	const uint32_t br = static_cast<uint32_t>((x + 1) + (z + 0) * mColumns);
	const uint32_t tr = static_cast<uint32_t>((x + 1) + (z + 1) * mColumns);

	const float u = position.x - x;
	const float v = position.z - z;

	float height = 0.0f;
	if (u > v) // bottom-right triangle
	{
		const auto& a = mesh.vertices[br];
		const auto& b = mesh.vertices[tr];
		const auto& c = mesh.vertices[bl];
		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * v) + (deltaAC * (1 - u));
	}
	else // top-left triangle
	{
		const auto& a = mesh.vertices[tl];
		const auto& b = mesh.vertices[tr];
		const auto& c = mesh.vertices[bl];
		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * u) + (deltaAC * (1 - v));
	}

	return height;
}
