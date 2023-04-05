#include <RAVEN/Inc/RAVEN.h>
#include "CylinderState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::CylinderState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

	mCylinder = MeshBuilder::CreateCylinderPC(2.0f, 0.5f, 6, 12);


	mMeshBuffer.Initialize(mCylinder.vertices.data(), sizeof(Graphics::VertexPC), (uint32_t)mCylinder.vertices.size(),
		mCylinder.indices.data(), (uint32_t)mCylinder.indices.size());

	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", Graphics::VertexPC::Format);

	mPixelShader.Initialize("../../Assets/Shaders/DoTransform.fx");

	mTransformBuffer.Initialize();

}

void RAVEN::CylinderState::Termiante()
{

	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();

}

void RAVEN::CylinderState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
	{
		MainApp().Quit();
		return;
	}

	if (inputSystem->IsKeyPressed(KeyCode::ONE))
		MainApp().ChangeState("CubeState");
	if (inputSystem->IsKeyPressed(KeyCode::TWO))
		MainApp().ChangeState("PlaneState");
	if (inputSystem->IsKeyPressed(KeyCode::FOUR))
		MainApp().ChangeState("SphereState");

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

void RAVEN::CylinderState::Render()
{

	mVertexShader.Bind();
	mPixelShader.Bind();

	//	Matrices msut be transposed before downloading to VRAM.
	TransformData transformData;
	transformData.worldMatrix = Transpose(Matrix4::RotationY(mRotation));
	transformData.viewMatrix = Transpose(mCamera.GetViewMatrix());
	transformData.projMatrix = Transpose(mCamera.GetProjectionMatrix());
	mTransformBuffer.Update(transformData);
	mTransformBuffer.BindVS(0);


	mMeshBuffer.Render();

}
