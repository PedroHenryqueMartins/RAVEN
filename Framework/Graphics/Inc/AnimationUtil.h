#pragma once
#include "Skeleton.h"

namespace RAVEN::Graphics::AnimationUtil
{
	using BoneTransforms = std::vector<Math::Matrix4>;
	using GetBoneTransform = std::function<Math::Matrix4(const Bone*)>;

	void ComputeBoneTransforms(const Skeleton& skeleton, GetBoneTransform getBoneTransform, BoneTransforms& boneTransforms);
	void ApplyBoneOffset(const Skeleton& skeleton, BoneTransforms& boneTransforms);

	void DrawSkeleton(const Skeleton& skeleton, const BoneTransforms& boneTransforms);
}