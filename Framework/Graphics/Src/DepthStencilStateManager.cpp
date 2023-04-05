#include "Precompiled.h"

#include "DepthStencilStateManager.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	std::unique_ptr<DepthStencilStateManager> sInstance;
}

void DepthStencilStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "DepthStencilStateManager -- System already initialized!");
	sInstance = std::make_unique<DepthStencilStateManager>();
	sInstance->Initialize();
}

void DepthStencilStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

DepthStencilStateManager* DepthStencilStateManager::Get()
{
	ASSERT(sInstance != nullptr, "DepthStencilStateManager -- No system registered.");
	return sInstance.get();
}

void DepthStencilStateManager::Initialize()
{
	mDepthStencilStates[0].Initialize(DepthStencilState::DepthTest::Enable, DepthStencilState::DepthWrite::Enable);
	mDepthStencilStates[1].Initialize(DepthStencilState::DepthTest::Enable, DepthStencilState::DepthWrite::Disable);
	mDepthStencilStates[2].Initialize(DepthStencilState::DepthTest::Disable, DepthStencilState::DepthWrite::Enable);
	mDepthStencilStates[3].Initialize(DepthStencilState::DepthTest::Disable, DepthStencilState::DepthWrite::Disable);
}

void DepthStencilStateManager::Terminate()
{
	for (auto& depthStencilState : mDepthStencilStates)
		depthStencilState.Terminate();
}

void DepthStencilStateManager::Set(DepthStencilState::DepthTest depthTest, DepthStencilState::DepthWrite depthWrite)
{
	const int index = static_cast<int>(depthTest) * 2 + static_cast<int>(depthWrite);
	mDepthStencilStates[index].Set();
}
