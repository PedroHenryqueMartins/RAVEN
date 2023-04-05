#include "Precompiled.h"
#include "Animation.h"

#include "Transform.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

Math::Vector3 Animation::GetPosition(float time) const
{
	//	If keyframe is empty use the default value, otherwise clamp the time and use the keyframes values.s
	//	If time is before the first frame or after the last frame, Clamp the time 

	//	TODO - Use 'time' to find the two keyframes you are in between
	//	i.e. the frame before and the frame after. Then lerp between
	//	these two keyframes if it is Position or scale or slerp between them if it is rotation
	//	percent = (time - frameBefore) / (frameAfter - frameBefore);

	if (mPositionKeys.empty())
		return Math::Vector3::Zero;
	if (mPositionKeys.size() == 1)
		return mPositionKeys[0].key;

	if (time < mPositionKeys[0].time)
		time = Math::Clamp(time, mPositionKeys[0].time, mPositionKeys.back().time);
	if (time > mPositionKeys.back().time)
		time = Math::Clamp(time, mPositionKeys[0].time, mPositionKeys.back().time);

	auto beforeFrame = mPositionKeys.front();
	auto afterFrame = mPositionKeys.back();

	for (int i = 0; i < mPositionKeys.size() - 1; ++i)
	{
		if (time >= mPositionKeys[i].time)
		{
			beforeFrame = mPositionKeys[i];
			afterFrame = mPositionKeys[i + 1];
		}
	}

	float percent = (time - beforeFrame.time) / (afterFrame.time - beforeFrame.time);

	Math::Vector3 result = Math::Lerp(beforeFrame.key, afterFrame.key, percent);
	return result;
	
}

Math::Quaternion Animation::GetRotation(float time) const
{
	if (mRotationKeys.empty())
		return Math::Quaternion::Identity;
	if (mRotationKeys.size() == 1)
		return mRotationKeys[0].key;

	if (time < mRotationKeys[0].time)
		time = Math::Clamp(time, mRotationKeys[0].time, mRotationKeys.back().time);
	if (time > mRotationKeys.back().time)
		time = Math::Clamp(time, mRotationKeys[0].time, mRotationKeys.back().time);

	auto beforeFrame = mRotationKeys.front();
	auto afterFrame = mRotationKeys.back();

	for (int i = 0; i < mRotationKeys.size() - 1; ++i)
	{
		if (time >= mRotationKeys[i].time)
		{
			beforeFrame = mRotationKeys[i];
			afterFrame = mRotationKeys[i + 1];
		}
	}

	float percent = (time - beforeFrame.time) / (afterFrame.time - beforeFrame.time);

	Math::Quaternion result = Math::Slerp(beforeFrame.key, afterFrame.key, percent);
	return result;
}

Math::Vector3 Animation::GetScale(float time) const
{
	if (mScaleKeys.empty())
		return Math::Vector3::One;
	if (mScaleKeys.size() == 1)
		return mScaleKeys[0].key;

	if (time < mScaleKeys[0].time)
		time = Math::Clamp(time, mScaleKeys[0].time, mScaleKeys.back().time);
	if (time > mScaleKeys.back().time)
		time = Math::Clamp(time, mScaleKeys[0].time, mScaleKeys.back().time);

	auto beforeFrame = mScaleKeys.front();
	auto afterFrame = mScaleKeys.back();

	for (int i = 0; i < mScaleKeys.size() - 1; ++i)
	{
		if (time >= mScaleKeys[i].time)
		{
			beforeFrame = mScaleKeys[i];
			afterFrame = mScaleKeys[i + 1];
		}
	}

	float percent = (time - beforeFrame.time) / (afterFrame.time - beforeFrame.time);

	Math::Vector3 result = Math::Lerp(beforeFrame.key, afterFrame.key, percent);
	return result;
}

Math::Matrix4 Animation::GetTransform(float time) const
{
	const auto& position = GetPosition(time);
	const auto& rotation = GetRotation(time);
	const auto& scale = GetScale(time);

	return Transform{ position, rotation, scale }.GetMatrix4();
}

float Animation::GetDuration() const
{
	//float duration = 0.0f;
	//float maxPositionKey = Math::Max(mPositionKeys.front().time, mPositionKeys.back().time);
	//float maxRotationKey = Math::Max(mRotationKeys.front().time, mRotationKeys.back().time);
	//float maxScaleKey = Math::Max(mScaleKeys.front().time, mScaleKeys.back().time);
	//duration = Math::Max(Math::Max(maxPositionKey, maxRotationKey), maxScaleKey);
	return Math::Max(mPositionKeys.back().time, Math::Max(mRotationKeys.back().time, mScaleKeys.back().time));
}