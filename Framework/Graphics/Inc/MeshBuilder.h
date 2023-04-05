#pragma once

#include "MeshTypes.h"

namespace RAVEN::Graphics
{
	

	class MeshBuilder
	{
	public:
		static MeshPC CreateCubePC();
		static MeshPC CreatePlanePC(float size, int rows = 16, int columns = 16);
		static MeshPC CreateCylinderPC(float height, float radius, int rings = 16, int slices = 16);
		static MeshPC CreateSpherePC(float radius, int rings = 16, int slices = 16);

		static MeshPX CreateSpherePX(float radius, int rings = 16, int slices = 16);
		static MeshPX CreateSkybox(float radius, int rings = 16, int slices = 16);

		static MeshPX CreateNDCQuad();

		static Mesh CreateSphere(float radius, int rings = 16, int slices = 16);
		static Mesh CreatePlane(float size, int rows = 16, int columns = 16, float tilling = 1.0f);

	};
}
