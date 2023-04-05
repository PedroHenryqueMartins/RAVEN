#include "GameState.h"
using namespace RAVEN;


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	
	MainApp().AddState<GameState>("GameState");

	MainApp().Run({ L"Hello Lighting" });

	return 0;
}