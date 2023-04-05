#include "Precompiled.h"
#include "AnimationUtil.h"

#include "SimpleDraw.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	void ComputeBoneTransformsRecursive(const Bone* bone, AnimationUtil::GetBoneTransform getBoneTransform, AnimationUtil::BoneTransforms& boneTransforms)
	{
		if (bone->parent)
		{
			boneTransforms[bone->index] = getBoneTransform(bone) * boneTransforms[bone->parentIndex];
		}
		else
		{
			boneTransforms[bone->index] = bone->toParentTransform;
		}

		for (auto child : bone->children)
			ComputeBoneTransformsRecursive(child, getBoneTransform, boneTransforms);
	}
}

void AnimationUtil::ComputeBoneTransforms(const Skeleton& skeleton , GetBoneTransform getBoneTransform, BoneTransforms& boneTransforms)
{
	boneTransforms.resize(skeleton.bones.size(), Math::Matrix4::Identity);
	ComputeBoneTransformsRecursive(skeleton.root, getBoneTransform, boneTransforms);
}

void AnimationUtil::ApplyBoneOffset(const Skeleton& skeleton, BoneTransforms& boneTransforms)
{
	for (auto& bone : skeleton.bones)
	{
		boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
	}
}

void AnimationUtil::DrawSkeleton(const Skeleton& skeleton, const BoneTransforms& boneTransforms)
{
	const size_t numBones = skeleton.bones.size();
	for (size_t i = 0; i < numBones; ++i)
	{
		const auto from = GetTranslation(boneTransforms[i]);
		//SimpleDraw::AddSphere(from, 0.01, Colors::Green, 8, 16);
		if (skeleton.bones[i]->parent)
		{
			const auto to = GetTranslation(boneTransforms[skeleton.bones[i]->parentIndex]);
			SimpleDraw::AddLine(from, to, Colors::DeepSkyBlue);
		}
	}
}