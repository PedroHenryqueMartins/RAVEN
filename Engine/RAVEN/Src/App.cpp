#include "Precompiled.h"
#include "App.h"

#include "AppState.h"

using namespace RAVEN;


void RAVEN::App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		LOG("App - Next state: %s", stateName.c_str());
		mNextState = iter->second.get();
	}
}

void App::Run(AppConfig appConfig)
{
	LOG("App - Running");

	LOG("App - Initializing Window");
	Core::Window window;

	window.Initialize(
		GetModuleHandle(nullptr), 
		appConfig.appName,
		appConfig.windowWidth, appConfig.windowHeight);

	LOG("App - Initialize engine systems..");
	auto handle = window.GetWindowHandle();
	Input::InputSystem::StaticInitialize(handle);
	Graphics::GraphicsSystem::StaticInitialize(handle, appConfig.fullScreen);
	Graphics::DepthStencilStateManager::StaticInitialize();
	Graphics::BlendStateManager::StaticInitialize();
	Graphics::TextureManager::StaticInitialize();
	Graphics::ModelManager::StaticInitialize();
	Graphics::DebugUI::StaticInitialize(handle, false, true);
	Graphics::SimpleDraw::StaticInitialize(appConfig.debugDrawLimit);

	ASSERT(mCurrentState, "App -- No app state found!");
	LOG("App - Initializing app state..");
	mCurrentState->Initialize();

	LOG("App - Entering game loop..");
	mRunning = true;
	while (mRunning)
	{
		window.ProcessMessage();
		if (!window.IsActive())
		{
			break; //Exit if our window is destroyed
		}

		if (mNextState)
		{
			mCurrentState->Termiante();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = Input::InputSystem::Get();
		inputSystem->Update(); //	Refresh input device state

		auto deltaTime = Core::TimeUtil::GetDeltaTimeSeconds();
		mCurrentState->Update(deltaTime);

		auto graphicsSystem = Graphics::GraphicsSystem::Get();
		graphicsSystem->BeginRender();

		//	TODO: EITHER ADD SpriteRender from XEngine or create the UIService, just like the RenderService
		//Graphics::SpriteRender::Get()->Begin();
		mCurrentState->Render();

		Graphics::DebugUI::BeginRender();
		mCurrentState->DebugUI();
		Graphics::DebugUI::EndRender();
		//	Do rendering here..

		graphicsSystem->EndRender();
	}

	LOG("Terminating app state..");
	mCurrentState->Termiante();

	//	Shutdown engine system
	LOG("App - Shutting down engine system..");
	Graphics::SimpleDraw::StaticTerminate();
	Graphics::DebugUI::StaticTerminate();
	Graphics::ModelManager::StaticTerminate();
	Graphics::TextureManager::StaticTerminate();
	Graphics::DepthStencilStateManager::StaticTerminate();
	Graphics::BlendStateManager::StaticTerminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();

	LOG("App - Terminating app window");
	window.Terminate();
}



void App::Quit()
{
	mRunning = false;
}