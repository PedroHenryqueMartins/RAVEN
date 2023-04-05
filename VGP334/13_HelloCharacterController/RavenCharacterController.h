#pragma once

#include <RAVEN/Inc/RAVEN.h>

class RavenCharacterController : public RAVEN::Graphics::Animator
{
public:

	enum class Direction
	{
		Idle, 
		Forward,
		Left,
		Right,
		Backward,
		BackwardLeft,
		BackwardRight,
		ForwardRight,
		ForwardLeft,
		Count
	};

	void Initialize(const RAVEN::Graphics::ModelID modelID);

	void Update(float deltaTime);

	void SetPosition(const RAVEN::Math::Vector3& position) { mPosition = position; }
	void SetDirection(const RAVEN::Math::Vector3& direction) { mDirection = direction; }

	const RAVEN::Math::Vector3& GetPosition() const { return mPosition; }
	const RAVEN::Math::Vector3& GetVelocity() const { return mVelocity; }
	const RAVEN::Math::Vector3& GetDirection() const { return mDirection; }

	const std::vector<RAVEN::Graphics::Animator>& GetAnimators() const { return mAnimators; }
	const RAVEN::Graphics::Animator& GetAnimator() const { return mAnimator; }

private:

	RAVEN::Math::Vector3 mPosition;
	RAVEN::Math::Vector3 mVelocity;
	RAVEN::Math::Vector3 mDirection;

	std::vector<RAVEN::Graphics::Animator> mAnimators;
	RAVEN::Graphics::Animator mAnimator;
	RAVEN::Graphics::ModelID mModelID;
};