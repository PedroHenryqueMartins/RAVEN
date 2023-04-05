#include <RAVEN/Inc/RAVEN.h>
#include "HeartState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;


void RAVEN::HeartState::Initialize()
{
	//	Top right part
	mVertices.push_back({ Vector3{  0.3f,  0.3f, 0.0f }, Colors::Red });
	mVertices.push_back({ Vector3{  0.6f,  0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ Vector3{  0.0f,  0.0f, 0.0f }, Colors::Red });
	//	Right part
	mVertices.push_back({ Vector3{  0.6f,  0.0f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{  0.5f, -0.6f, 0.0f }, Colors::Red });
	mVertices.push_back({ Vector3{  0.0f,  0.0f, 0.0f }, Colors::Red });
	// Top left part
	mVertices.push_back({ Vector3{ -0.6f,  0.0f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{ -0.3f,  0.3f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{  0.0f,  0.0f, 0.0f }, Colors::Red});
	// Left Part
	mVertices.push_back({ Vector3{ -0.5f,  -0.6f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{ -0.6f,  0.0f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{  0.0f,  0.0f, 0.0f }, Colors::Red});

	//	Middle Part
	mVertices.push_back({ Vector3{  0.0f,  0.0f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{  0.5f, -0.6f, 0.0f }, Colors::Red});
	mVertices.push_back({ Vector3{ -0.5f, -0.6f, 0.0f }, Colors::Red});

	//	Bottom part
	mVertices.push_back({ Vector3{ -0.5f,  -0.6f, 0.0f }, Colors::Red });
	mVertices.push_back({ Vector3{  0.5f,  -0.6f, 0.0f }, Colors::Red });
	mVertices.push_back({ Vector3{  0.0f,  -0.9f, 0.0f }, Colors::Red });


	mMeshBuffer.Initialize(mVertices.data(), sizeof(Graphics::VertexPC), (uint32_t)mVertices.size());

	mVertexShader.Initialize("../../Assets/Shaders/DoColors.fx", Graphics::VertexPC::Format);

	mPixelShader.Initialize("../../Assets/Shaders/DoColors.fx");
}

void RAVEN::HeartState::Termiante()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void RAVEN::HeartState::Update(float deltaTime)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		MainApp().Quit();
	}

	if (GetAsyncKeyState('2'))
		MainApp().ChangeState("NewShapeState");
	if (GetAsyncKeyState('3'))
		MainApp().ChangeState("GameState");
}

void RAVEN::HeartState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();
}
