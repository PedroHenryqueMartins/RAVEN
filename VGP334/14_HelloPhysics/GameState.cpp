#include <RAVEN/Inc/RAVEN.h>
#include "GameState.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Input;
using namespace RAVEN::Math;
using namespace RAVEN::Physics;

//namespace
//{
//
//	float RandomFloat(float min, float max)
//	{
//		return std::uniform_real_distribution<float>{min, max}(myRandomEngine);
//	}
//}

//	NOTE TO SELF: Try testing the character movement within the wind and the gravity. Use simpledraw to test first, a simple cube or the tetrahedron and how the wind and gravity affect it.

void RAVEN::GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 3.0f, -3.0f});
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mStandardEffect.Initialize();
	mStandardEffect.SetCamera(&mCamera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(&mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.SetDepthBias(0.00001f);

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(&mDirectionalLight);

	PhysicsWorld::Settings settings
	{
		{0.0f, -9.8f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		false,
		1.0f / 60.0f,
		0.0f,
		10,
	};

	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddPlane({ Vector3::YAxis, 0.0f });

	mStandardEffect.Initialize();
	mShadowEffect.Initialize();

	const char* fileName = "../../Assets/Models/MawMonster/mawmonster.model";
	ModelIO::LoadModel(fileName, mModel);
	ModelIO::LoadMaterial(fileName, mModel);
	mModelRG = CreateRenderGroup(mModel);

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	mDirectionalLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
}

void RAVEN::GameState::Termiante()
{
	CleanUpRenderGroup(mModelRG);

	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
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

	if (inputSystem->IsKeyPressed(KeyCode::PAUSE))
		mPause = !mPause;
	if (mPause)
		deltaTime = 0.0f;

	mPhysicsWorld.Update(deltaTime);
}

void RAVEN::GameState::Render()
{
	if (!mTetrahedrons.empty())
	{
		mShadowEffect.Begin();
		for (auto& tetrahedron : mTetrahedrons)
		{
			auto worldMatrix = tetrahedron.GetWorldMatrix();
			auto translation = GetTranslation(worldMatrix);
			auto rotation = Quaternion::RotationEuler({ 0.0f, -1.57f, 0.0f }) * Quaternion::RotationMatrix(worldMatrix);
			for (auto& robj : mModelRG)
			{
				robj.transform.position = translation;
				robj.transform.rotation = rotation;
			}

			DrawRenderGroup(mShadowEffect, mModelRG);
		}
		mShadowEffect.End();

		mStandardEffect.Begin();
		for (auto& tetrahedron : mTetrahedrons)
		{
			auto worldMatrix = tetrahedron.GetWorldMatrix();
			auto translation = GetTranslation(worldMatrix);
			auto rotation = Quaternion::RotationEuler({ 0.0f, -1.57f, 0.0f }) * Quaternion::RotationMatrix(worldMatrix);
			for (auto& robj : mModelRG)
			{
				robj.transform.position = translation;
				robj.transform.rotation = rotation;
			}
			
			DrawRenderGroup(mStandardEffect, mModelRG);
		}
		mStandardEffect.End();
	}
	

	mPhysicsWorld.DebugDraw();

	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::AddGroundPlane(10.0f, Colors::White);
	SimpleDraw::Render(mCamera);
}

void RAVEN::GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}

		ImGui::Separator();

		ImGui::ColorEdit3("Light Ambient", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit3("Light Diffuse", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit3("Light Specular", &mDirectionalLight.specular.r);
	}

	ImGui::Separator;

	mStandardEffect.DebugUI();

	ImGui::Separator;

	mShadowEffect.DebugUI();

	ImGui::Separator;

	if (ImGui::Button("Spawn Boom!"))
	{
		mPhysicsWorld.Clear();
		mTetrahedrons.clear();
		for (int i = 0; i < 100; ++i)
		{
			Math::Vector3 vel = Random::OnUnitSphere() * 0.1f;
			vel.y = Abs(vel.y);

			auto particle = mPhysicsWorld.AddParticle();
			particle->SetPosition({ 0.0f, 2.0f, 0.0f });
			particle->SetVelocity(vel);
			particle->radius = 1.0f;
			particle->invMass = 1.0f;
			particle->bounce = Random::UniformFloat(0.5f, 0.95f);
		}
	}
	
	ImGui::Separator;

	if (ImGui::CollapsingHeader("Sticks!", ImGuiTreeNodeFlags_DefaultOpen))
	{

		ImGui::DragFloat3("Particle 1 Position", &particle1Pos.x, 0.1f, -10.0f, 10.0f);

		ImGui::DragFloat3("Particle 2 Position", &particle2Pos.x, 0.1f, -10.0f, 10.0f);

		if (ImGui::Button("Spawn Sticks!"))
		{
			mPhysicsWorld.Clear();
			mTetrahedrons.clear();
			for (int i = 0; i < 100; ++i)
			{
				Math::Vector3 vel1 = Random::OnUnitSphere() * 0.1f;
				vel1.y = Abs(vel1.y);
				Math::Vector3 vel2 = Random::OnUnitSphere() * 0.1f;
				vel2.y = Abs(vel2.y);

				auto particle1 = mPhysicsWorld.AddParticle();
				particle1->SetPosition(particle1Pos);
				particle1->SetVelocity(vel1);
				particle1->radius = 0.02f;
				particle1->invMass = 1.0f;

				auto particle2 = mPhysicsWorld.AddParticle();
				particle2->SetPosition(particle2Pos);
				particle2->SetVelocity(vel2);
				particle2->radius = 0.02f;
				particle2->invMass = 1.0f;

				mPhysicsWorld.AddConstraint<Physics::Spring>(particle1, particle2);
			}
		}
	}

	if (ImGui::Button("Cube!"))
	{
		mPhysicsWorld.Clear();
		mTetrahedrons.clear();
		for (int i = 0; i < 50; ++i)
		{
			auto p1 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p2 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p3 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p4 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p5 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p6 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p7 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p8 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			p1->SetPosition({ -0.5f, -0.5f, -0.5f });
			p2->SetPosition({ -0.5f, +0.5f, -0.5f });
			p3->SetPosition({ +0.5f, +0.5f, -0.5f });
			p4->SetPosition({ +0.5f, -0.5f, -0.5f });
			p5->SetPosition({ -0.5f, -0.5f, +0.5f });
			p6->SetPosition({ -0.5f, +0.5f, +0.5f });
			p7->SetPosition({ +0.5f, +0.5f, +0.5f });
			p8->SetPosition({ +0.5f, -0.5f, +0.5f });

			Vector3 vel1 = Random::OnUnitSphere() * 1.0f;
			vel1.y = Abs(vel1.y) * 3.0f;
			p1->SetVelocity(vel1);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p3);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p4);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p1);
			
			mPhysicsWorld.AddConstraint<Physics::Spring>(p5, p6);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p6, p7);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p7, p8);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p8, p5);
			
			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p5);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p6);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p7);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p8);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p7);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p8);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p5);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p6);
			
		}
	}

	ImGui::Separator;

	if (ImGui::Button("Ball and Chain!"))
	{
		mPhysicsWorld.Clear();

		Math::Vector3 position{ 0.0f, 6.0f, 0.0f };
		Physics::Particle* last = nullptr;

		for (int i = 0; i < 10; ++i)
		{
			auto p = mPhysicsWorld.AddParticle();
			p->SetPosition(position);
			p->SetVelocity(Vector3::Zero);
			p->radius = (i == 9) ? 0.2f : 0.02f;
			p->invMass = (i == 9) ? 0.1f : 1.0f;

			if (i == 0)
				mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(p);
			else
				mPhysicsWorld.AddConstraint<Physics::Spring>(p, last);

			position.x += 0.5f;
			last = p;
		}
	}
	
	if (ImGui::Button("Tetrahedron!"))
	{
		mPhysicsWorld.Clear();
		mTetrahedrons.clear();

		for (int i = 0; i < 50; ++i)
		{
			auto p1 = mPhysicsWorld.AddParticle();
			p1->SetPosition({ 0.0f, 1.0f, 0.0f });
			p1->radius = 0.02f;
			p1->invMass = 1.0f;
			auto p2 = mPhysicsWorld.AddParticle();
			p2->SetPosition({ 0.0f, 0.8f, 0.1f });
			p2->radius = 0.02f;
			p2->invMass = 1.0f;
			auto p3 = mPhysicsWorld.AddParticle();
			p3->SetPosition({ 0.1f, 0.8f, -0.1f });
			p3->radius = 0.02f;
			p3->invMass = 1.0f;
			auto p4 = mPhysicsWorld.AddParticle();
			p4->SetPosition({ -0.1f, 0.8f, -0.1f });
			p4->radius = 0.02f;
			p4->invMass = 1.0f;

			Vector3 vel1 = Random::OnUnitSphere() * 0.25f;
			vel1.y = Abs(vel1.y) * 3.0f;
			p1->SetVelocity(vel1);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p3, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p4, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p3, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p4, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p2, 0.2f);
		}
	}

	if (ImGui::Button("Model"))
	{
		mPhysicsWorld.Clear();
		mTetrahedrons.clear();
		mCubes.clear();
		mCloth.clear();

		for (int i = 0; i < 50; ++i)
		{
			auto& t = mTetrahedrons.emplace_back(mPhysicsWorld.AddTetrahedron());
			t.SetPosition({ 0.0f, 1.0f, 0.0f });
			Vector3 vel = Random::OnUnitSphere() * 0.25f;
			vel.y = Abs(vel.y) * 3.0f;
			t.SetVelocity(vel);
			t.a->SetVelocity(Random::OnUnitSphere() * 0.25f);
		}
	}

	ImGui::Separator;

	if (ImGui::Button("New Cube Test"))
	{
		mPhysicsWorld.Clear();
		mCubes.clear();
		mTetrahedrons.clear();
		mCloth.clear();

		for (int i = 0; i < 10; ++i)
		{
			auto& c = mCubes.emplace_back(mPhysicsWorld.AddCube());
			c.SetPosition({ 0.0f, 2.0f, 0.0f });
			Vector3 vel = Random::OnUnitSphere() * 0.25f;
			vel.y = Abs(vel.y) * 3.0f;
			c.SetVelocity(vel);
		}
	}

	ImGui::Separator;

	if (ImGui::Button("Cloth!"))
	{
		mPhysicsWorld.Clear();
		mTetrahedrons.clear();
		mCloth.clear();
		mCubes.clear();

		const int maxX = 11;
		const int maxY = 9;

		for (int i = 0; i < maxY; ++i)
		{
			for (int j = 0; j < maxX; ++j)
			{
				auto& p = mCloth.emplace_back(mPhysicsWorld.AddParticle());
				p->SetPosition(Math::Vector3{ j - (maxX - 1) / 2.0f, 20.0f, (i - (maxY - 1) / 2.0f) } * 0.1f);
				p->SetVelocity(Math::Vector3::Zero);
				p->radius = 0.02f;
				p->invMass = 1.0f;
			}
		}

		//	Bottom Constraints
		mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(mCloth[maxX * (maxY - 9) + (maxX -5)]);
		mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(mCloth[maxX * (maxY - 9) + (maxX - 7)]);
		//	VerticalEdge Constraints
		mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(mCloth[maxX * (maxY - 1) + (maxX - 11)]);
		mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(mCloth[maxX * (maxY - 1) + (maxX - 1)]);
		//	Top Constraints
		mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(mCloth[maxX * (maxY - 1) + (maxX - 10)]);
		mPhysicsWorld.AddConstraint<Physics::FixedConstraint>(mCloth[maxX * (maxY - 1) + (maxX - 2)]);

		for (int i = 0; i < maxY; ++i)
		{
			for (int j = 0; j < maxX; ++j)
			{

				if (i < maxY - 1)
				{
					mPhysicsWorld.AddConstraint<Physics::Spring>(mCloth[maxX * i + j], mCloth[maxX * (i + 1) + j], 0.0f);
					if (j > 0)
					{
						mPhysicsWorld.AddConstraint<Physics::Spring>(mCloth[maxX * i + j], mCloth[maxX * (i + 1) + (j - 1)], 0.0f);
					}
					if (j < maxX - 1)
					{
						mPhysicsWorld.AddConstraint<Physics::Spring>(mCloth[maxX * i + j], mCloth[maxX * (i + 1) + (j + 1)], 0.0f);
					}
				}
				if (j < maxX - 1)
				{
					mPhysicsWorld.AddConstraint<Physics::Spring>(mCloth[maxX * i + j], mCloth[maxX * i + (j + 1)], 0.0f);
				}
			}
		}
	}

	ImGui::End();

	mPhysicsWorld.DebugUI();
}
