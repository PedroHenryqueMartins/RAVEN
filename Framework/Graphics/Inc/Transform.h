#pragma once

#include "Common.h"

namespace RAVEN::Graphics
{
	struct Transform
	{
		RAVEN::Math::Vector3 position = RAVEN::Math::Vector3::Zero;
		RAVEN::Math::Quaternion rotation = Math::Quaternion::Identity;
		RAVEN::Math::Vector3 scale = RAVEN::Math::Vector3::One;

		[[nodiscard]] Math::Matrix4 GetMatrix4() const
		{
			Math::Matrix4 matTrans = Math::Matrix4::Translation(position);
			Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(rotation);
			Math::Matrix4 matScale = Math::Matrix4::Scaling(scale);
			return matScale * matRot * matTrans;
		}
	};
}