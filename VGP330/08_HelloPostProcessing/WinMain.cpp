#include "GameState.h"

using namespace RAVEN;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig appConfig;
	appConfig.appName = L"Hello PostProcessing";
	appConfig.windowWidth = 1280;
	appConfig.windowHeight = 720;
	appConfig.debugDrawLimit = 900000;
	MainApp().AddState<GameState>("GameState");

	MainApp().Run(appConfig);

	return 0;
}