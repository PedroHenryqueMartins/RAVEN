#include "RavenCharacterController.h"

using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;


void RavenCharacterController::Initialize(ModelID modelID)
{
	
 	Animator::Initialize(modelID);
	mModelID = modelID;
	mAnimator.Initialize(mModelID);
	
	mAnimators.resize(Direction::ForwardLeft);

	for (size_t i = 0; i < mAnimators.size(); ++i)
	{
		mAnimators[i].Initialize(mModelID);
		mAnimators[i].PlayAnimation(i, true);
	}

	mAnimator.PlayAnimation(Idle, true);
}

void RavenCharacterController::Update(float deltaTime)
{
	const float acceleration = 30.0f;
	const float turnSpeed = 1.0f;
	const float maxSpeed = 9.0f;
	auto inputs = InputSystem::Get();
	if (inputs->IsKeyDown(KeyCode::W) || inputs->GetLeftAnalogY(0) > 0.0f)
	{
		mVelocity += mDirection * acceleration * deltaTime;
		if (inputs->IsKeyPressed(KeyCode::W))
			mAnimator.PlayAnimation(Forward, true);
	}
	if (inputs->IsKeyDown(KeyCode::S) || inputs->GetLeftAnalogY(0) < 0.0f)
	{
		mVelocity -= mDirection * acceleration * deltaTime;
		if (inputs->IsKeyPressed(KeyCode::S))
			mAnimator.PlayAnimation(Backward, true);
	}
	if (inputs->IsKeyDown(KeyCode::D) || inputs->GetLeftAnalogX(0) > 0.0f)
	{
		mVelocity += Cross(Vector3::YAxis, mDirection) * acceleration * deltaTime;
		if (inputs->IsKeyPressed(KeyCode::D))
			mAnimator.PlayAnimation(Right, true);
	}
	if (inputs->IsKeyDown(KeyCode::A) || inputs->GetLeftAnalogX(0) < 0.0f)
	{
		mVelocity -= Cross(Vector3::YAxis, mDirection) * acceleration * deltaTime;
		if (inputs->IsKeyPressed(KeyCode::A))
			mAnimator.PlayAnimation(Left, true);
	}

	mPosition += mVelocity * deltaTime;
	mVelocity *= 0.65f;

	if (MagnitudeSqr(mVelocity) > Sqr(maxSpeed))
	{
		mVelocity = Normalize(mVelocity) * maxSpeed;
	}

	mAnimator.Update(deltaTime);

	Vector3 zero = Vector3::Zero;
	if (mVelocity == zero)
		PlayAnimation(Idle, true);

	/*const float acceleration = 30.0f;
	const float turnSpeed = 1.0f;
	const float maxSpeed = 9.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W) || inputSystem->GetLeftAnalogY(0) > 0.0f)
	{
		mVelocity += mDirection * acceleration * deltaTime;
	}
	else if (inputSystem->IsKeyDown(KeyCode::S) || inputSystem->GetLeftAnalogY(0) < 0.0f)
	{
		mVelocity -= mDirection * acceleration * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::D) || inputSystem->GetLeftAnalogX(0) > 0.0f)
	{
		mVelocity += Cross(Vector3::YAxis, mDirection) * acceleration * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::A) || inputSystem->GetLeftAnalogX(0) < 0.0f)
	{
		mVelocity -= Cross(Vector3::YAxis, mDirection) * acceleration * deltaTime;
	}

	mPosition += mVelocity * deltaTime;
	mVelocity *= 0.95f;

	if (MagnitudeSqr(mVelocity) > Sqr(maxSpeed))
		mVelocity = Normalize(mVelocity) * maxSpeed;

	for (auto& animator : mAnimators)
		animator.Update(deltaTime);

	const auto& idlePose = mAnimators[1];
	const size_t size = idlePose.GetAnimClip(1).boneAnimations.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (idlePose.GetAnimClip(1).boneAnimations[i] == nullptr)
		{
			mBoneTranslations.push_back(RAVEN::Math::Vector3::Zero);
		}
		else
		{
			mBoneTranslations.push_back(idlePose.GetAnimClip(1).boneAnimations[i].get()->GetPosition(deltaTime));
		}
	}
	
	for (size_t i = 0; i < size; ++i)
	{
		if (idlePose.GetAnimClip(1).boneAnimations[i] == nullptr)
		{
			mBoneRotations.push_back(RAVEN::Math::Quaternion::Identity);
		}
		else
		{
			mBoneRotations.push_back(idlePose.GetAnimClip(1).boneAnimations[i].get()->GetRotation(deltaTime));
		}
	}

	for (size_t i = 0; i < size; ++i)
	{
		if (idlePose.GetAnimClip(1).boneAnimations[i] == nullptr)
		{
			mBoneScales.push_back(RAVEN::Math::Vector3::One);
		}
		else
		{
			mBoneScales.push_back(idlePose.GetAnimClip(1).boneAnimations[i].get()->GetScale(deltaTime));
		}
	}

	if (MagnitudeSqr(mVelocity) > 0.0f)
	{
		auto direction = Normalize(mVelocity);
		float angle = atan2(direction.x, direction.z);
		float percent = angle / Constants::Pi;

		const Animator* startBlend = nullptr;
		const Animator* endBlend = nullptr;
		float blendWeight = 0.0f;

		if (percent >= 0.0f)
		{
			if (percent < 0.25f)
			{
				startBlend = &mAnimators[Direction::Forward];
				endBlend = &mAnimators[Direction::ForwardRight];
				blendWeight = percent / 0.25f;
			}
			else if (percent < 0.5f)
			{
				startBlend = &mAnimators[Direction::ForwardRight];
				endBlend = &mAnimators[Direction::Right];
				blendWeight = (percent - 0.25f) / 0.25f;
			}
			else if (percent < 0.75f)
			{
				startBlend = &mAnimators[Direction::Right];
				endBlend = &mAnimators[Direction::BackwardRight];
				blendWeight = (percent - 0.5f) / 0.25f;
			}
			else
			{
				startBlend = &mAnimators[Direction::BackwardRight];
				endBlend = &mAnimators[Direction::Backward];
				blendWeight = (percent - 0.75f) / 0.25f;
			}
		}
		else
		{
			percent = -percent;
			if (percent < 0.25f)
			{
				startBlend = &mAnimators[Direction::Forward];
				endBlend = &mAnimators[Direction::ForwardLeft];
				blendWeight = percent / 0.25f;
			}
			else if (percent < 0.5f)
			{
				startBlend = &mAnimators[Direction::ForwardLeft];
				endBlend = &mAnimators[Direction::Left];
				blendWeight = (percent - 0.25f) / 0.25f;
			}
			else if (percent < 0.75f)
			{
				startBlend = &mAnimators[Direction::Left];
				endBlend = &mAnimators[Direction::BackwardLeft];
				blendWeight = (percent - 0.5f) / 0.25f;
			}
			else
			{
				startBlend = &mAnimators[Direction::BackwardLeft];
				endBlend = &mAnimators[Direction::Backward];
				blendWeight = (percent - 0.75f) / 0.25f;
			}
		}

		float speedPercent = Magnitude(mVelocity) / maxSpeed;
		float speedWeight = 1.0f;
		if (speedPercent < 0.5f)
			speedWeight = speedPercent / 0.5f;

		const auto& startTranslations = startBlend->GetBoneTranslations();
		const auto& startRotations = startBlend->GetBoneRotations();
		const auto& startScales = startBlend->GetBoneScales();

		const auto& endTranslations = endBlend->GetBoneTranslations();
		const auto& endRotations = endBlend->GetBoneRotations();
		const auto& endScales = endBlend->GetBoneScales();

		const auto skeleton = GetModel().skeleton.get();
		for (size_t i = 0; i < skeleton->bones.size(); ++i)
		{
			mBoneTranslations[i] = mBoneTranslations[i] * (1.0f - speedWeight) + (startTranslations[i] * (1.0f - blendWeight) + endTranslations[i] * blendWeight) * speedWeight;
			mBoneRotations[i] = mBoneRotations[i] * (1.0f - speedWeight) + (startRotations[i] * (1.0f - blendWeight) + endRotations[i] * blendWeight) * speedWeight;
			mBoneScales[i] = mBoneScales[i] * (1.0f - speedWeight) + (startScales[i] * (1.0f - blendWeight) + endScales[i] * blendWeight) * speedWeight;
		}
	}*/
}
