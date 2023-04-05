#pragma once

#include "Keyframe.h"

namespace RAVEN::Graphics
{
	class Animation
	{
	public:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;
		Math::Matrix4 GetTransform(float time) const;

		float GetDuration() const;

	private:
		friend class AnimationIO;
		friend class AnimationBuilder;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
	};
}