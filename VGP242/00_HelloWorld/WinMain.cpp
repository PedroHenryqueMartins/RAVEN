#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LOG("Hello, I am %s", "Pedro");
	Sleep(1000);
	LOG("Good night, I am awake! %d seconds have passed.", 1);
	Sleep(2500);
	LOG("%f seconds have passed", 2.5f);

	const int n = 42;
	ASSERT(n == 7, "%d is not 7!", n);

	return 0;
}