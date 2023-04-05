#include "Precompiled.h"
#include "GroundPlaneComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

void GroundPlaneComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	
	auto textureManager = TextureManager::Get();
	mGroundPlane.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mGroundPlane.material.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mGroundPlane.material.specular = { 0.75f, 0.75f, 0.75f, 1.0f };
	mGroundPlane.material.power = 10.0f;
	mGroundPlane.diffuseMapID = textureManager->LoadTexture("../../Assets/Images/dirt_seamless.jpg");
	mGroundPlane.meshBuffer.Initialize(MeshBuilder::CreatePlane(mSize, 10, 10, 10.0f));

}

void GroundPlaneComponent::Terminate()
{

}