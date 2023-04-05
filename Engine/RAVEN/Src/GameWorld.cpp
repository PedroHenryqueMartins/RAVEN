#include "Precompiled.h"
#include "GameWorld.h"

#include "GameObjectFactory.h"

using namespace RAVEN;

void GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameWorld -- Already Initialized.");
	mInitialized = true;

	for (auto& service : mServices)
		service->Initialize();

	mGameObjectSlots.resize(capacity);
	mFreeSlots.resize(capacity);
	std::iota(mFreeSlots.rbegin(), mFreeSlots.rend(), 0);

}

void GameWorld::Terminate()
{
	ASSERT(!mUpdating, "Game World - Cannot terminate world during update.");

	if (!mInitialized)
		return;

	//	Destroy all remaining game objects
	for (auto gameObject : mUpdateList)
		DestroyGameObject(gameObject->GetHandle());

	ProcessDestroyList();
	ASSERT(mUpdateList.empty() && mToBeDestroyed.empty(), "Game World -- Failed to clean up all game objects.");

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld -- Already updating the world.");

	//	This defers game object destruction
	mUpdating = true;

	for (auto& service : mServices)
		service->Update(deltaTime);

	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (IsValid(gameObject->GetHandle()))
			gameObject->Update(deltaTime);
	}

	//	Allow game object destruction;
	mUpdating = false;

	//	Now we can safely destroy game objects
	ProcessDestroyList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
		service->Render();
}

void GameWorld::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
	
}

GameObject* GameWorld::CreateGameObject(std::filesystem::path templateFile)
{
	ASSERT(mInitialized, "GameWorld -- World must be initialized.");

	if (mFreeSlots.empty())
		return nullptr;

	const uint32_t freeSlot = mFreeSlots.back();
	mFreeSlots.pop_back();

	//	Create new object at this slot
	auto& slot = mGameObjectSlots[freeSlot];
	auto& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();

	//	Attach components
	GameObjectFactory::Make(templateFile, *newObject);

	//	Initialize handle
	GameObjectHandle handle;
	handle.mIndex = freeSlot;
	handle.mGeneration = slot.generation;

	//Initialize game object
	newObject->mWorld = this;
	newObject->mHandle = handle;
	if (strcmp(newObject->GetName().c_str(), "Unnamed") == 0)
	{
		auto objectName = "GameObject" + std::to_string(handle.mIndex);
		newObject->SetName(objectName.c_str());
	}
	newObject->Initialize();

	//	Register with the updated list
	return mUpdateList.emplace_back(newObject.get());
}

GameObject* GameWorld::GetGameObject(GameObjectHandle handle)
{
	if (!IsValid(handle))
		return nullptr;

	return mGameObjectSlots[handle.mIndex].gameObject.get();
}

GameObject* RAVEN::GameWorld::GetGameObject(const char* gameObjectName)
{
	for (auto& gameObject : mUpdateList)
	{
		if (strcmp(gameObject->GetName().c_str(), gameObjectName) == 0)
		{
			return mGameObjectSlots[gameObject->mHandle.mIndex].gameObject.get();
		}
	}
}

void GameWorld::DestroyGameObject(GameObjectHandle handle)
{
	if (!IsValid(handle))
		return;

	//	Increment generation
	auto& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++; //	This invalidates all existing handles to this slot
	mToBeDestroyed.push_back(handle.mIndex);

	
}

bool RAVEN::GameWorld::IsValid(const char* gameObjectName)
{
	auto gameObjectIndex = GetGameObject(gameObjectName)->GetHandle().mIndex;
	if (gameObjectIndex  < 0 || gameObjectIndex >= mGameObjectSlots.size())
		return false;
	if (mGameObjectSlots[gameObjectIndex].generation != GetGameObject(gameObjectName)->GetHandle().mGeneration)
		return false;

	return true;
}

bool GameWorld::IsValid(GameObjectHandle handle) const
{
	if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
		return false;

	if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
		return false;

	return true;
}

void GameWorld::ProcessDestroyList()
{
	ASSERT(!mUpdating, "Game World -- Cannot destroy game object during update.");

	for (auto index : mToBeDestroyed)
	{
		auto& slot = mGameObjectSlots[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "Game World -- Game Object is stil valid! Cannot be destroyed yet.");

		//	Remove from update list
		mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject));

		//	Terminate game object
		gameObject->Terminate();

		//	Delete this game object and recycle the slot.
		slot.gameObject.reset();
		mFreeSlots.push_back(index);
	}

	mToBeDestroyed.clear();
}
