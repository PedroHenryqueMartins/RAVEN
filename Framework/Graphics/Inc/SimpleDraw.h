#pragma once

#include "Colors.h"

namespace RAVEN::Graphics
{
	class Camera;
}

namespace RAVEN::Graphics::SimpleDraw
{
	void StaticInitialize(uint32_t maxVertexCount = 10000);
	void StaticTerminate();

	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, Color color);

	void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, Color color);
	
	void AddAABB(const Math::Vector3& center, const Math::Vector3& extend, Color color, bool fill = false);

	void AddSphere(const Math::Vector3& center, float radius, Color color, size_t rings = 10, size_t slices = 20);

	void AddTransform(const Math::Matrix4& transform);

	void AddGroundPlane(float size, const Color& color = Colors::DarkGray);

	void Render(const Camera& camera);


	// For a challenge, you can add also:
	// - AddCylinder
	// - AddCone
	// - AddCapsule

}