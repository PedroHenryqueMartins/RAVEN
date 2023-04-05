#pragma once

#include "MeshTypes.h"

namespace RAVEN::Graphics
{
	class Terrain final
	{
	public:
		void Initialize(std::filesystem::path fileName, float heightScale);

		float GetHeight(const Math::Vector3& position) const;

		Mesh mesh;

		size_t mRows = 0;
		size_t mColumns = 0;
	};
}