#include "Precompiled.h"
#include "RenderObject.h"

#include "Model.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}

RenderGroup Graphics::CreateRenderGroup(const Model& model, const Animator* animator)
{
	RenderGroup renderGroup;	
	renderGroup.reserve(model.meshData.size());

	auto TryLoad = [](const auto& textureName) -> TextureID
	{
		if (textureName.empty())
		{
			return 0;
		}

		return TextureManager::Get()->LoadTexture(textureName);
	};

	for (auto& meshData : model.meshData)
	{
		auto& renderObject = renderGroup.emplace_back();

		auto& materialData = model.materialData[meshData.materialIndex];
		renderObject.material = materialData.material;
		renderObject.diffuseMapID = TryLoad(materialData.diffuseMapName);
		renderObject.specularMapID = TryLoad(materialData.specularMapName);
		renderObject.displacementMapID = TryLoad(materialData.displacementMapName);
		renderObject.normalMapID = TryLoad(materialData.normalMapName);

		renderObject.meshBuffer.Initialize(meshData.mesh);
		
		renderObject.animator = animator;
		renderObject.skeleton = model.skeleton.get();
	}

	return renderGroup;
}

void Graphics::CleanUpRenderGroup(RenderGroup& renderGroup)
{
	for (auto& renderObject : renderGroup)
		renderObject.Terminate();
}
