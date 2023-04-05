#pragma once

#include "ModelManager.h"

namespace RAVEN::Graphics
{
	class Animator
	{
	public:

		void Initialize(ModelID modelID);

		void PlayAnimation(int clipIndex, bool looping);

		void Update(float deltaTime);
		bool IsFinished() const;

		Math::Matrix4 GetToParentTransform(const Bone* bone) const;
		const int GetAnimClip() const { return mClipIndex; }
		const float GetAnimationTick() const { return mAnimationTick; }
		void SetAnimClip(int value) { mClipIndex = value; }
		const AnimationClip& GetAnimClip(int clipIndex) const;
		float mAnimationTick = 0.0f;
	protected:
		const Model& GetModel() const;

	private:
		
		ModelID mModelID;
		//Model* mModel = nullptr;
		int mClipIndex = -1;
		int mNextClipIndex = -1;
		
		float mBlendWeight = 0.0f;
		bool mLooping = false;
	};
}