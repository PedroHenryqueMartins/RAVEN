#include "Precompiled.h"
#include "ModelManager.h"

#include "ModelIO.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sInstance;
}

void ModelManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "Model Manager - Manager Already Initialized.");
	sInstance = std::make_unique<ModelManager>();
}

void ModelManager::StaticTerminate()
{
	// Reset is used to avoid memory leak.
	sInstance.reset();
}

ModelManager* ModelManager::Get()
{
	ASSERT(sInstance != nullptr, "Model Manager - Manager Not Initialized.");
	return sInstance.get();
}

ModelID ModelManager::LoadModel(std::filesystem::path fileName)
{
	const auto modelID = GetModelID(fileName);
	//	Structure Binding
	auto [iter, success] = mInventory.insert({ modelID, nullptr });
	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(fileName, *modelPtr);
		ModelIO::LoadMaterial(fileName, *modelPtr);
		ModelIO::LoadSkeleton(fileName, *modelPtr);
		ModelIO::LoadAnimation(fileName, *modelPtr);
	}
	return modelID;
}

void ModelManager::AddAnimation(ModelID modelID, std::filesystem::path fileName)
{
	const auto iter = mInventory.find(modelID);
	if (iter != mInventory.end())
	{
		auto& modelPtr = iter->second;
		ModelIO::LoadAnimation(fileName, *modelPtr);

	}
}

const Model* ModelManager::GetModel(ModelID id) const
{
	const auto iter = mInventory.find(id);
	if (iter == mInventory.end())
		return nullptr;
	return iter->second.get();
}

ModelID ModelManager::GetModelID(const std::filesystem::path& fileName) const
{
	const auto modelID = std::filesystem::hash_value(fileName);
	return modelID;
}
