#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;


void RAVEN::GameState::Initialize()
{
	
	float theta = 0.0f;
	float delta = 10.0f * Math::Constants::DegToRad;
	float r = 0.3f;
	int numSegments = 20;

	for (int i = 0; i < numSegments; ++i)
	{
		float x0 = r * std::sin(theta);
		float y0 = r * std::cos(theta);
		float x1 = r * std::sin(theta + delta);
		float y1 = r * std::cos(theta + delta);
		mVertices.push_back({ Vector3{0.0f, 0.0f, 0.0f}, Colors::Blue });
		mVertices.push_back({ Vector3{x0, y0, 0.0f}, Colors::Blue });
		mVertices.push_back({ Vector3{x1, y1, 0.0f}, Colors::Blue });
		theta += delta;
	}

	mMeshBuffer.Initialize(mVertices.data(), sizeof(Graphics::VertexPC), (uint32_t)mVertices.size());

	mVertexShader.Initialize("../../Assets/Shaders/DoColors.fx", Graphics::VertexPC::Format);

	mPixelShader.Initialize("../../Assets/Shaders/DoColors.fx");

	

}

void RAVEN::GameState::Termiante()
{
	
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
	
}

void RAVEN::GameState::Update(float deltaTime)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		MainApp().Quit();
	}
	if (GetAsyncKeyState('1'))
		MainApp().ChangeState("HeartState");

	if (GetAsyncKeyState('2'))
		MainApp().ChangeState("NewShapeState");
}

void RAVEN::GameState::Render()
{
	
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();

}
