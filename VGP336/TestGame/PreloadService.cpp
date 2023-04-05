#include "PreloadService.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;
using namespace RAVEN::Physics;

void PreloadService::Initialize()
{
	const char* fileNames[] =
	{
		"../../Assets/Models/CharController/charController.model",
	};

	const std::vector<const char*> animationNames[] =
	{
		{
			"../../Assets/Models/CharController/charControllerIdle.animset",
			"../../Assets/Models/CharController/charControllerForward.animset"
		}
		
	};

	auto TryLoadTexture = [](const auto& textureName) -> TextureID
	{
		if (textureName.empty())
			return 0;
		return TextureManager::Get()->LoadTexture(textureName);
	};

	auto modelManager = ModelManager::Get();
	auto textureManager = ModelManager::Get();

	for (int i = 0; i < std::size(fileNames); ++i)
	{
		auto fileName = fileNames[i];
		auto modelID = modelManager->LoadModel(fileName);
		for (const auto& animationFileName : animationNames[i])
			modelManager->AddAnimation(modelID, animationFileName);

		auto model = modelManager->GetModel(modelID);
		for (const auto& materialData : model->materialData)
		{
			TryLoadTexture(materialData.diffuseMapName);
			TryLoadTexture(materialData.specularMapName);
			TryLoadTexture(materialData.displacementMapName);
			TryLoadTexture(materialData.normalMapName);
		}
	}
}