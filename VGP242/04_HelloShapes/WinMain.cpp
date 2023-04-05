#include "GameState.h"
#include "HeartState.h"
#include "NewShapeState.h"

using namespace RAVEN;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	// Method #1 = Initialize config directly
	/*RAVEN::AppConfig config;
	config.appName = L"Hello DirectX";
	config.windowWidth = 800;
	config.windowHeight = 600;
	RAVEN::MainApp().Run(config);
	*/

	//	Method #2 - Default Initialization
	//	RAVEN::MainApp().Run({ });

	//	Method #3 = Aggregate Initialization
	//	RAVEN::MainApp().Run({ L"Hello DirectX" });
	
	
	MainApp().AddState<HeartState>("HeartState");
	MainApp().AddState<NewShapeState>("NewShapeState");
	MainApp().AddState<GameState>("GameState");
	MainApp().Run({ L"Hello Shapes" });


	return 0;
}