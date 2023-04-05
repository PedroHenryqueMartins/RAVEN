#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;

void RAVEN::GameState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });

	mCube = MeshBuilder::CreateCubePC();
	mPlane = MeshBuilder::CreatePlanePC(1.0f, 7, 20);
	mCylinder = MeshBuilder::CreateCylinderPC(2.0f, 0.45f, 5, 16);
	mSphere = MeshBuilder::CreateSpherePC(1.0f, 16, 16);
	mSphere2 = MeshBuilder::CreateSpherePX(1.0f, 16, 16);


	mCubeMeshBuffer.Initialize(mCube);
	mPlaneMeshBuffer.Initialize(mPlane);
	mCylinderMeshBuffer.Initialize(mCylinder);
	mSphereMeshBuffer.Initialize(mSphere);
	mSphereMeshBuffer2.Initialize(mSphere2);

	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTransform.fx");

	mTexturingVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", Graphics::VertexPX::Format);
	mTexturingPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");

	mTexture.Initialize("../../Assets/Images/earth.jpg");
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	mTransformBuffer.Initialize();

}

void RAVEN::GameState::Termiante()
{

	mTransformBuffer.Terminate();

	mTexture.Terminate();
	mSampler.Terminate();

	mTexturingVertexShader.Terminate();
	mTexturingPixelShader.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mSphereMeshBuffer.Terminate();
	mCylinderMeshBuffer.Terminate();
	mPlaneMeshBuffer.Terminate();
	mCubeMeshBuffer.Terminate();

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

	mVertexShader.Bind();
	mPixelShader.Bind();

	TransformData transformData;
	transformData.viewMatrix = Transpose(mCamera.GetViewMatrix());
	transformData.projMatrix = Transpose(mCamera.GetProjectionMatrix());
	mTransformBuffer.BindVS(0);

	Matrix4 translation = Matrix4::Translation(mPosition + Vector3(-4.0f, 0.0f, 0.0f));
	Matrix4 rotationX = Matrix4::RotationX(mRotation.x);
	Matrix4 rotationY = Matrix4::RotationY(mRotation.y);
	Matrix4 rotationZ = Matrix4::RotationZ(mRotation.z);
	Matrix4 scale = Matrix4::Scaling(mScale);
	Matrix4 world = scale * rotationX * rotationY * rotationZ * translation;

	
	//	Matrices msut be transposed before downloading to VRAM.
	transformData.worldMatrix = Transpose(world);
	mTransformBuffer.Update(transformData);
	mCubeMeshBuffer.Render();
	
	translation = Matrix4::Translation(mPosition + Vector3(-2.0f, 0.0f, 0.0f));
	world = scale * rotationX * rotationY * rotationZ * translation;
	transformData.worldMatrix = Transpose(world);
	mTransformBuffer.Update(transformData);
	mPlaneMeshBuffer.Render();

	translation = Matrix4::Translation(mPosition + Vector3(0.0f, 0.0f, 0.0f));
	world = scale * rotationX * rotationY * rotationZ * translation;
	transformData.worldMatrix = Transpose(world);
	mTransformBuffer.Update(transformData);
	mCylinderMeshBuffer.Render();

	translation = Matrix4::Translation(mPosition + Vector3(2.0f, 0.0f, 0.0f));
	world = scale * rotationX * rotationY * rotationZ * translation;
	transformData.worldMatrix = Transpose(world);
	mTransformBuffer.Update(transformData);
	mSphereMeshBuffer.Render();


	mTexturingVertexShader.Bind();
	mTexturingPixelShader.Bind();
	mTexture.BindPS();
	mSampler.BindPS();

	translation = Matrix4::Translation(mPosition + Vector3(4.0f, 0.0f, 0.0f));
	world = scale * rotationX * rotationY * rotationZ * translation;
	transformData.worldMatrix = Transpose(world);
	mTransformBuffer.Update(transformData);
	mSphereMeshBuffer2.Render();
}
