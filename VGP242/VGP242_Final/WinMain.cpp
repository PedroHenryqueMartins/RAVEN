#include "GameState.h"
#include "CylinderState.h"
#include "PlaneState.h"
#include "SphereState.h"

using namespace RAVEN;


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	//	Use 1, 2, 3 and 4 to swap between cube, plane, cylinder and sphere.

	MainApp().AddState<GameState>("GameState");
	

	MainApp().Run({ L"Hello Final" });

	return 0;
}