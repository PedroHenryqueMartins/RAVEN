#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	template <class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float time)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= time, "AnimationBuilder -- Cannot add keyframe back in time.");
		keyframes.emplace_back(value, time);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& position, float time)
{
	PushKey(mWorkingCopy.mPositionKeys, position, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion& rotation, float time)
{
	PushKey(mWorkingCopy.mRotationKeys, rotation, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time)
{
	PushKey(mWorkingCopy.mScaleKeys, scale, time);
	return *this;
}

Animation AnimationBuilder::Get()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty(), "AnimationBuilder -- Animation has no position keys.");
	ASSERT(!mWorkingCopy.mRotationKeys.empty(), "AnimationBuilder -- Animation has no rotation keys.");
	ASSERT(!mWorkingCopy.mScaleKeys.empty(), "AnimationBuilder -- Animation has no scale keys.");
	return std::move(mWorkingCopy);
}
