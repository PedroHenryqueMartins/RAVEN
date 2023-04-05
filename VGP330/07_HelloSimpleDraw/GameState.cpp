#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });
	auto graphicSystem = Graphics::GraphicsSystem::Get();
	graphicSystem->SetClearColor(Graphics::Colors::GhostWhite);
	

}

void RAVEN::GameState::Termiante()
{
	

}

void RAVEN::GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		MainApp().Quit();
		return;
	}

	const float moveSpeed = 10.0f;
	const float turnSpeed = 10.0f * Constants::DegToRad;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::E))
		mCamera.Rise(moveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::Q))
		mCamera.Rise(-moveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnSpeed * deltaTime);
	}

	
	
}

void RAVEN::GameState::Render()
{
	Matrix4 viewMatrix = mCamera.GetViewMatrix();
	Matrix4 projMatrix = mCamera.GetProjectionMatrix();

	Matrix4 worldMatrix = Matrix4::Translation(mCamera.GetPosition());

	
	SimpleDraw::AddAABB(Vector3{ 3.0f, 1.0f, 1.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Colors::Blue, false);
	SimpleDraw::AddAABB(Vector3{ 7.0f, 1.0f, 1.0f }, Vector3{ 1.0f, 1.0f, 1.0f }, Colors::Red, true);

	SimpleDraw::AddGroundPlane(50);
	SimpleDraw::AddTransform(Math::Matrix4::Identity);

	SimpleDraw::AddSphere(Math::Vector3::Zero, 2.0f, Colors::Blue, 8, 16);

	SimpleDraw::Render(mCamera);

}

void RAVEN::GameState::DebugUI()
{
	

}
