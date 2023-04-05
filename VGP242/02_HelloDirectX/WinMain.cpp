#include <RAVEN/Inc/RAVEN.h>

using namespace RAVEN;

class GameStateOne : public RAVEN::AppState
{
public:

	void Initialize() override
	{
		auto graphicsSystem = Graphics::GraphicsSystem::Get();
		graphicsSystem->SetClearColor(Graphics::Colors::BlanchedAlmond);
	}

	void Update(float deltaTime) override
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			MainApp().Quit();
			//Exit if ESC key is pressed
		}

		if (GetAsyncKeyState('2'))
			MainApp().ChangeState("GameStateTwo");

	}
};

class GameStateTwo : public RAVEN::AppState
{
public:

	void Initialize() override
	{
		auto graphicsSystem = Graphics::GraphicsSystem::Get();
		graphicsSystem->SetClearColor(Graphics::Colors::BlueViolet);
	}

	void Update(float deltaTime) override
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			MainApp().Quit();
			 //Exit if ESC key is pressed
		}

		if (GetAsyncKeyState('1'))
			MainApp().ChangeState("GameStateOne");
	}
};

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
	
	
	MainApp().AddState<GameStateOne>("GameStateOne");
	MainApp().AddState<GameStateTwo>("GameStateTwo");
	MainApp().Run({ L"Hello DirectX" });


	return 0;
}