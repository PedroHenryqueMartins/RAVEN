#include "Precompiled.h"

#include "Animator.h"
#include "Transform.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

void Animator::Initialize(ModelID modelID)
{
	mModelID = modelID;
}


void Animator::PlayAnimation(int clipIndex, bool looping)
{
	const auto& model = GetModel();
	ASSERT(clipIndex < model.animationClips.size(), "Animator -- Invalid clip index.");

	//	Playing anything
	if (mClipIndex == -1)
	{
		mClipIndex = clipIndex;
		mAnimationTick = 0;
		mLooping = looping;
	}
	else
	{
		mNextClipIndex = clipIndex;
		mBlendWeight = 0.0f;
	}
}


void Animator::Update(float deltaTime)
{
	const float acceleration = 30.0f;
	const float turnSpeed = 1.0f;
	const float maxSpeed = 9.0f;

	const auto& animClip = GetAnimClip(mClipIndex);
	mAnimationTick += animClip.ticksPerSecond * deltaTime;
	if (mAnimationTick > animClip.tickDuration)
	{
		if (mLooping)
		{
			while (mAnimationTick > animClip.tickDuration)
			{
				mAnimationTick -= animClip.tickDuration;
			}
		}
		else
			mAnimationTick = animClip.tickDuration;
	}

	if (mNextClipIndex != -1)
	{
		mBlendWeight += deltaTime;
		if (mBlendWeight >= 1.0f)
		{
			mClipIndex = mNextClipIndex;
			mNextClipIndex = -1;
		}
	}
	
}

bool Animator::IsFinished() const
{
	if (mLooping)
		return false;

	const auto& animClip = GetAnimClip(mClipIndex);
	if (mAnimationTick < animClip.tickDuration)
	{
		return false;
	}
	
	return true;
}

Math::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	Math::Matrix4 toParentTransform = bone->toParentTransform;
	const auto& fromAnimClip = GetAnimClip(mClipIndex);
	const auto& fromBoneAnimation = fromAnimClip.boneAnimations[bone->index];

	if (mNextClipIndex != -1)
	{
		
		const auto& toAnimClip = GetAnimClip(mNextClipIndex);
		const auto& toBoneAnimation = toAnimClip.boneAnimations[bone->index];

		Math::Vector3 fromPos = Math::GetTranslation(bone->toParentTransform);
		Math::Quaternion fromRot = Math::Quaternion::RotationMatrix(bone->toParentTransform);
		Math::Vector3 fromScale = Math::Vector3::One;

		if (fromBoneAnimation != nullptr)
		{
			fromPos = fromBoneAnimation->GetPosition(mAnimationTick);
			fromRot = fromBoneAnimation->GetRotation(mAnimationTick);
			fromScale = fromBoneAnimation->GetScale(mAnimationTick);
		}

		Math::Vector3 toPos = Math::GetTranslation(bone->toParentTransform);
		Math::Quaternion toRot = Math::Quaternion::RotationMatrix(bone->toParentTransform);
		Math::Vector3 toScale = Math::Vector3::One;

		if (toBoneAnimation != nullptr)
		{
			toPos = toBoneAnimation->GetPosition(mAnimationTick);
			toRot = toBoneAnimation->GetRotation(mAnimationTick);
			toScale = toBoneAnimation->GetScale(mAnimationTick);
		}

		

		auto position = Math::Lerp(fromPos, toPos, mBlendWeight);
		auto rotation = Math::Slerp(fromRot, toRot, mBlendWeight);
		auto scale = Math::Lerp(fromScale, toScale, mBlendWeight);
		toParentTransform = Transform{ position, rotation, scale }.GetMatrix4();
	}
	//	Check if this bone is animated
	else
	{
		if (fromBoneAnimation != nullptr)
			toParentTransform  = fromBoneAnimation->GetTransform(mAnimationTick);
	}

	//posFrom = boneAnimationFrom->GetPosition();
	//posTo = boneAnimationTo->GetPosition();
	//Lerp(posFrom, posTo, transitionWeight);

	return toParentTransform;
}

const Model& Animator::GetModel() const
{
	auto modelManager = ModelManager::Get();
	return *modelManager->GetModel(mModelID);
}

const AnimationClip& Animator::GetAnimClip(int clipIndex) const
{
	return GetModel().animationClips[clipIndex];
}
