#include "Precompiled.h"
#include "TerrainService.h"

using namespace RAVEN;

void TerrainService::Terminate()
{
	mTerrainRenderObject.Terminate();
}

void TerrainService::DebugUI()
{
	if (ImGui::CollapsingHeader("Terrain Service", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Terrain", &mTerrainRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Terrain", &mTerrainRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Terrain", &mTerrainRenderObject.material.specular.r);
		ImGui::DragFloat("Power##Terrain", &mTerrainRenderObject.material.power, 1.0f, 1.0f, 100.0f);
	}
}

void TerrainService::LoadTerrain(const char* fileName, float maxHeight)
{
	mTerrain.Initialize(fileName, maxHeight);

	mTerrainRenderObject.material.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mTerrainRenderObject.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mTerrainRenderObject.material.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mTerrainRenderObject.material.power = 10.0f;
	mTerrainRenderObject.meshBuffer.Initialize(mTerrain.mesh);
}

void TerrainService::LoadTexture(const char* fileName, uint32_t index)
{
	// HACK: pchan - using diffuse as 0 and specular as 1. If we want to
	// share RenderObject between different effects, we need to store textures
	// in a generic way.
	auto tm = Graphics::TextureManager::Get();
	if (index == 0)
		mTerrainRenderObject.diffuseMapID = tm->LoadTexture("../../Assets/Images/grass_2048.jpg");
	else
		mTerrainRenderObject.specularMapID = tm->LoadTexture("../../Assets/Images/dirt_seamless.jpg");
}

float TerrainService::GetHeight(const Math::Vector3& position) const
{
	return mTerrain.GetHeight(position);
}
