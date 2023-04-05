#include "Precompiled.h"
#include "AnimatorComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "ModelComponent.h"
#include "RenderService.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void AnimatorComponent::Initialize()
{
	auto modelComponent = GetOwner().GetComponent<ModelComponent>();

	//	This assumes the model component is initialized first!
	mAnimator.Initialize(modelComponent->GetModelID());
	mAnimator.PlayAnimation(1, true);
	
}

void AnimatorComponent::Update(float deltaTime)
{
	mAnimator.Update(deltaTime);
}

void AnimatorComponent::DebugUI()
{

}
