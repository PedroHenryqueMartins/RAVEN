#include "Precompiled.h"
#include "TransformComponent.h"

using namespace RAVEN;

void TransformComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
}