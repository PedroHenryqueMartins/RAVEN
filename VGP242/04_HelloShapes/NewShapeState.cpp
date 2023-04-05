#include "RAVEN/Inc/RAVEN.h"
#include "NewShapeState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;


void RAVEN::NewShapeState::Initialize()
{

	//	Symbol
	mVertices.push_back({ Vector3{ -0.05f, -0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ Vector3{ -0.025f,  -0.4f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ Vector3{ -0.05f, -0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Yellow });
	mVertices.push_back({ Vector3{ -0.025f,  -0.6f, 0.0f }, Colors::Yellow });

	//	Handle part (vertical)
	mVertices.push_back({ Vector3{ -0.05f, -0.8f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{ -0.05f, -0.5f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{  0.0f, -0.8f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{ -0.05f, -0.5f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{  0.0f, -0.5f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{  0.0f, -0.8f, 0.0f }, Colors::Brown });

	// Handle part (horizontal)
	mVertices.push_back({ Vector3{ -0.15f, -0.5f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{ -0.15f, -0.4f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{  0.10f, -0.4f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{  0.10f, -0.4f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{  0.10f, -0.5f, 0.0f }, Colors::Brown });
	mVertices.push_back({ Vector3{ -0.15f, -0.5f, 0.0f }, Colors::Brown });

	//	Blade LeftPart
	mVertices.push_back({ Vector3{ -0.10f, -0.4f, 0.0f }, Colors::Silver });
	mVertices.push_back({ Vector3{ -0.10f,  0.4f, 0.0f }, Colors::Silver });
	mVertices.push_back({ Vector3{  0.05f,  -0.4f, 0.0f }, Colors::AntiqueWhite });
	//	Blade RightPart
	mVertices.push_back({ Vector3{ -0.10f,  0.4f, 0.0f }, Colors::Silver });
	mVertices.push_back({ Vector3{  0.05f,  0.4f, 0.0f }, Colors::Silver });
	mVertices.push_back({ Vector3{  0.05f, -0.4f, 0.0f }, Colors::AntiqueWhite });

	//	Blade Tip
	mVertices.push_back({ Vector3{  0.05f,  0.4f, 0.0f }, Colors::Silver });
	mVertices.push_back({ Vector3{ -0.10f,  0.4f, 0.0f }, Colors::Silver });
	mVertices.push_back({ Vector3{  0.05f,  0.9f, 0.0f }, Colors::DarkBlue });


	mMeshBuffer.Initialize(mVertices.data(), sizeof(Graphics::VertexPC), (uint32_t)mVertices.size());


	mVertexShader.Initialize("../../Assets/Shaders/DoColors.fx", Graphics::VertexPC::Format);

	mPixelShader.Initialize("../../Assets/Shaders/DoColors.fx");
}

void RAVEN::NewShapeState::Termiante()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void RAVEN::NewShapeState::Update(float deltaTime)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		MainApp().Quit();
	}

	if (GetAsyncKeyState('1'))
		MainApp().ChangeState("HeartState");

	if (GetAsyncKeyState('3'))
		MainApp().ChangeState("GameState");
}

void RAVEN::NewShapeState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();
}
