#pragma once

namespace RAVEN::Math
{
	struct Plane
	{
		Vector3 normal = Vector3::YAxis;
		float distance = 0.0f;
	};
}