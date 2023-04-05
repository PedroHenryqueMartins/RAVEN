#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	RAVEN::Core::Window myWindow;

	myWindow.Initialize(instance, L"Hello Window", 1280, 720);

	while (true)
	{
		myWindow.ProcessMessage();
		if (!myWindow.IsActive())
		{
			break; //Exit if our window is destroyed
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break; //Exit if ESC key is pressed
		}
	}

	myWindow.Terminate();
	return 0;
}