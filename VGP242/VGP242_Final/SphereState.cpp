#include <RAVEN/Inc/RAVEN.h>
#include "SphereState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::SphereState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

	mSphere = MeshBuilder::CreateSpherePC(5.0f, 16, 16);


	mMeshBuffer.Initialize(mSphere.vertices.data(), sizeof(Graphics::VertexPC), (uint32_t)mSphere.vertices.size(),
		mSphere.indices.data(), (uint32_t)mSphere.indices.size());

	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", Graphics::VertexPC::Format);

	mPixelShader.Initialize("../../Assets/Shaders/DoTransform.fx");

	mTransformBuffer.Initialize();

}

void RAVEN::SphereState::Termiante()
{

	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();

}

void RAVEN::SphereState::Update(float deltaTime)
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
	if (inputSystem->IsKeyPressed(KeyCode::THREE))
		MainApp().ChangeState("CylinderState");

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

void RAVEN::SphereState::Render()
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
