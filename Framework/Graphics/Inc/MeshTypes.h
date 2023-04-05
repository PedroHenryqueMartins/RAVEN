#pragma once

#include "VertexTypes.h"

namespace RAVEN::Graphics
{
	template <class VertexT>
	struct MeshBase
	{
		using VertexType = VertexT;
		std::vector<VertexT> vertices;
		std::vector<uint32_t> indices;
	};

	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using Mesh = MeshBase<Vertex>;
}
