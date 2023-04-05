#include "Precompiled.h"

#include "BlendStateManager.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	std::unique_ptr<BlendStateManager> sInstance;
}

void BlendStateManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "BlendStateManager -- System already initialized!");
	sInstance = std::make_unique<BlendStateManager>();
	sInstance->Initialize();
}

void BlendStateManager::StaticTerminate()
{
	if (sInstance != nullptr)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

BlendStateManager* BlendStateManager::Get()
{
	ASSERT(sInstance != nullptr, "BlendStateManager -- No system registered.");
	return sInstance.get();
}

void BlendStateManager::Initialize()
{
	mBlendStates[0].Initialize(BlendState::Mode::Opaque);
	mBlendStates[1].Initialize(BlendState::Mode::AlphaBlend);
	mBlendStates[2].Initialize(BlendState::Mode::AlphaPremultiplied);
	mBlendStates[3].Initialize(BlendState::Mode::Additive);
}

void BlendStateManager::Terminate()
{
	for (auto& blendStates : mBlendStates)
		blendStates.Terminate();
}

void BlendStateManager::Set(BlendState::Mode blendMode)
{
	const int index = static_cast<int>(blendMode);
	mBlendStates[index].Set();
}
