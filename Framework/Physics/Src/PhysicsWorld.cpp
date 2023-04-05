#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Physics;

namespace
{
	//	Tetrahedron constants
	constexpr Math::Vector3 oa{ +0.0f, 0.8165f, +0.0000f};
	constexpr Math::Vector3 ob{ -0.5f, 0.0000f, -0.2887f };
	constexpr Math::Vector3 oc{ +0.5f, 0.0000f, -0.2887f };
	constexpr Math::Vector3 od{ +0.5f, 0.0000f, +0.5774f };
}

namespace CubeConstants
{
	constexpr Math::Vector3 oa{ -0.5f, 0.0000f,  0.0000f };
	constexpr Math::Vector3 ob{  0.5f, 0.0000f,  0.0000f };
	constexpr Math::Vector3 oc{  0.5f, 0.5000f,  0.0000f };
	constexpr Math::Vector3 od{ -0.5f, 0.5000f,  0.0000f };

	constexpr Math::Vector3 oe{ -0.5f, 0.5000f, 0.5000f };
	constexpr Math::Vector3 of{  0.5f, 0.5000f, 0.5000f };
	constexpr Math::Vector3 og{  0.5f, 0.0000f, 0.5000f };
	constexpr Math::Vector3 oh{ -0.5f, 0.0000f, 0.5000f };
}

void Cloth::SetPosition(const Math::Vector3& pos)
{
	a->SetPosition(pos);
}

void Cloth::SetVelocity(const Math::Vector3& velocity)
{
	a->SetVelocity(velocity);
}

void Tetrahedron::SetPosition(const Math::Vector3& pos)
{
	a->SetPosition(pos + oa);
	b->SetPosition(pos + ob);
	c->SetPosition(pos + oc);
	d->SetPosition(pos + od);
}

void Tetrahedron::SetVelocity(const Math::Vector3& velocity)
{
	a->SetVelocity(velocity);
	b->SetVelocity(velocity);
	c->SetVelocity(velocity);
	d->SetVelocity(velocity);
}

Math::Vector3 Tetrahedron::GetLook() const
{
	Math::Vector3 pos = GetPosition();
	return Math::Normalize(d->position - pos);
}

Math::Vector3 Tetrahedron::GetRight() const
{
	return Math::Normalize(c->position - b->position);
}

Math::Vector3 Tetrahedron::GetUp() const
{
	Math::Vector3 pos = GetPosition();
	return Math::Normalize(a->position - pos);
}

Math::Vector3 Tetrahedron::GetPosition() const
{
	return (b->position + c->position + d->position) * 0.3333f;
}

Math::Matrix4 Tetrahedron::GetWorldMatrix() const
{
	Math::Vector3 l = GetLook();
	Math::Vector3 r = GetRight();
	Math::Vector3 u = GetUp();
	Math::Vector3 p = GetPosition();

	return
	{
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		l.x, l.y, l.z, 0.0f,
		p.x, p.y, p.z, 1.0f
	};
}

void Cube::SetPosition(const Math::Vector3& pos)
{
	a->SetPosition(pos + CubeConstants::oa);
	b->SetPosition(pos + CubeConstants::ob);
	c->SetPosition(pos + CubeConstants::oc);
	d->SetPosition(pos + CubeConstants::od);
	e->SetPosition(pos + CubeConstants::oe);
	f->SetPosition(pos + CubeConstants::of);
	g->SetPosition(pos + CubeConstants::og);
	h->SetPosition(pos + CubeConstants::oh);
}

void Cube::SetVelocity(const Math::Vector3& velocity)
{
	a->SetVelocity(velocity);
	b->SetVelocity(velocity);
	c->SetVelocity(velocity);
	d->SetVelocity(velocity);
	e->SetVelocity(velocity);
	f->SetVelocity(velocity);
	g->SetVelocity(velocity);
	h->SetVelocity(velocity);
}

Math::Vector3 Cube::GetLook() const
{
	Math::Vector3 pos = GetPosition();
	return Math::Normalize(a->position - pos);
}

Math::Vector3 Cube::GetRight() const
{
	return Math::Normalize(b->position - a->position);
}

Math::Vector3 Cube::GetUp() const
{
	Math::Vector3 pos = GetPosition();
	return Math::Normalize(a->position - pos);
}

Math::Vector3 Cube::GetPosition() const
{
	return (a->position + b->position + g->position + h->position) * 0.25f;
}

Math::Matrix4 Cube::GetWorldMatrix() const
{
	Math::Vector3 l = GetLook();
	Math::Vector3 r = GetRight();
	Math::Vector3 u = GetUp();
	Math::Vector3 p = GetPosition();

	return
	{
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		l.x, l.y, l.z, 0.0f,
		p.x, p.y, p.z, 1.0f
	};
}

void PhysicsWorld::Initialize(Settings settings)
{
	mSettings = std::move(settings);
}

void PhysicsWorld::Update(float deltaTime)
{
	if (mPause)
		return;

	mTimer += deltaTime;
	//	This loops makes sure to make a fixed update of a 60 frame.
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;

		AccumulateForces(); //	If there are different forces in the world e.g. wind, gravity. What is affecting the bodies, get the updated acceleration.
		Integrate(); //	Once you have the new acceleration, use the verlet integration.
		SatisfyConstraints();	//	Check if someone is breaking any rule, checking for example is something colliding with a wall.
	}

	
}

void PhysicsWorld::DebugDraw() const
{
	for (auto& c : mConstraints)
		c->DebugDraw();

	for (auto& p : mParticles)
	{
		//auto color = Math::Lerp(Colors::Cyan, Colors::Red, Math::Clamp(p->radius / 0.25f, 0.0f, 1.0f));
		//SimpleDraw::AddAABB(p->position, p->radius, Colors::Cyan, false);
		SimpleDraw::AddSphere(p->position, p->radius, Colors::Cyan, 5, 4);
	}
}

void PhysicsWorld::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("Pause Simulation", &mPause);
	ImGui::Checkbox("Wind", &mSettings.useWind);
	ImGui::DragFloat("Gravity", &mSettings.gravity.y, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat3("Wind Direction", &mSettings.wind.x, 0.1f, -1000.0f, 1000.0f);
	if (ImGui::Button("Reset Gravity"))
		mSettings.gravity.y = -9.8f;
	if (ImGui::Button("Reset Wind"))
	{
		mSettings.wind.x = 0.0f;
		mSettings.wind.y = 0.0f;
		mSettings.wind.z = 0.0f;

	}
	ImGui::End();
}

Tetrahedron PhysicsWorld::AddTetrahedron()
{
	Tetrahedron tetrahedron;
	tetrahedron.a = AddParticle();
	tetrahedron.b = AddParticle();
	tetrahedron.c = AddParticle();
	tetrahedron.d = AddParticle();

	tetrahedron.a->position = { +0.0f, 0.8165f, +0.0000f};
	tetrahedron.b->position = { -0.5f, 0.0000f, -0.2887f };
	tetrahedron.c->position = { +0.5f, 0.0000f, -0.2887f };
	tetrahedron.d->position = { +0.5f, 0.0000f, +0.5774f };

	tetrahedron.a->radius = 0.02f;
	tetrahedron.b->radius = 0.02f;
	tetrahedron.c->radius = 0.02f;
	tetrahedron.d->radius = 0.02f;

	tetrahedron.a->invMass = 1.0f;
	tetrahedron.b->invMass = 1.0f;
	tetrahedron.c->invMass = 1.0f;
	tetrahedron.d->invMass = 1.0f;

	AddConstraint<Spring>(tetrahedron.a, tetrahedron.b);
	AddConstraint<Spring>(tetrahedron.a, tetrahedron.c);
	AddConstraint<Spring>(tetrahedron.a, tetrahedron.d);

	AddConstraint<Spring>(tetrahedron.b, tetrahedron.c);
	AddConstraint<Spring>(tetrahedron.c, tetrahedron.d);
	AddConstraint<Spring>(tetrahedron.d, tetrahedron.b);

	return tetrahedron;
}

Cube PhysicsWorld::AddCube()
{
	Cube cube;
	cube.a = AddParticle();
	cube.b = AddParticle();
	cube.c = AddParticle();
	cube.d = AddParticle();
	cube.e = AddParticle();
	cube.f = AddParticle();
	cube.g = AddParticle();
	cube.h = AddParticle();

	cube.a->position = { -0.5f, 0.0000f,  0.0000f };
	cube.b->position = {  0.5f, 0.0000f,  0.0000f };
	cube.c->position = {  0.5f, 0.5000f,  0.0000f };
	cube.d->position = { -0.5f, 0.5000f,  0.0000f };

	cube.e->position = { -0.5f, 0.5000f, 0.5000f };
	cube.f->position = {  0.5f, 0.5000f, 0.5000f };
	cube.g->position = {  0.5f, 0.0000f, 0.5000f };
	cube.h->position = { -0.5f, 0.0000f, 0.5000f };

	cube.a->radius = 0.02f;
	cube.b->radius = 0.02f;
	cube.c->radius = 0.02f;
	cube.d->radius = 0.02f;
	cube.e->radius = 0.02f;
	cube.f->radius = 0.02f;
	cube.g->radius = 0.02f;
	cube.h->radius = 0.02f;

	cube.a->invMass = 1.0f;
	cube.b->invMass = 1.0f;
	cube.c->invMass = 1.0f;
	cube.d->invMass = 1.0f;
	cube.e->invMass = 1.0f;
	cube.f->invMass = 1.0f;
	cube.g->invMass = 1.0f;
	cube.h->invMass = 1.0f;

	AddConstraint<Spring>(cube.a, cube.b);
	AddConstraint<Spring>(cube.a, cube.c);
	AddConstraint<Spring>(cube.a, cube.d);
	AddConstraint<Spring>(cube.a, cube.e);
	AddConstraint<Spring>(cube.a, cube.h);
	AddConstraint<Spring>(cube.a, cube.g);

	AddConstraint<Spring>(cube.b, cube.c);
	AddConstraint<Spring>(cube.b, cube.d);
	AddConstraint<Spring>(cube.b, cube.g);
	AddConstraint<Spring>(cube.b, cube.h);

	AddConstraint<Spring>(cube.g, cube.h);
	AddConstraint<Spring>(cube.g, cube.f);
	AddConstraint<Spring>(cube.g, cube.e);

	AddConstraint<Spring>(cube.h, cube.b);
	AddConstraint<Spring>(cube.h, cube.d);
	AddConstraint<Spring>(cube.h, cube.e);
	AddConstraint<Spring>(cube.h, cube.f);

	AddConstraint<Spring>(cube.d, cube.c);
	AddConstraint<Spring>(cube.d, cube.e);
	AddConstraint<Spring>(cube.d, cube.f);

	AddConstraint<Spring>(cube.c, cube.e);
	AddConstraint<Spring>(cube.c, cube.f);
	AddConstraint<Spring>(cube.c, cube.g);

	AddConstraint<Spring>(cube.e, cube.f);
	AddConstraint<Spring>(cube.f, cube.b);

	return cube;
}

void PhysicsWorld::AddPlane(const Math::Plane& plane)
{
	mPlanes.push_back(plane);
}

void PhysicsWorld::Clear()
{
	mConstraints.clear();
	mParticles.clear();
}

void PhysicsWorld::AccumulateForces()
{
	if (mSettings.useWind)
	{
		for (auto& p : mParticles)
		{
			p->acceleration.x = mSettings.gravity.x + mSettings.wind.x;
			p->acceleration.y = mSettings.gravity.y + mSettings.wind.y;
			p->acceleration.z = mSettings.gravity.z + mSettings.wind.z;
		}
	}
	else
	{
		for (auto& p : mParticles)
		{
			p->acceleration = mSettings.gravity;
		}
	}
	
}
void PhysicsWorld::Integrate()
{
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto& p : mParticles)
	{
		const auto newPosition = (p->position * 2.0f) - p->lastPosition + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = newPosition;
	}
}
void PhysicsWorld::SatisfyConstraints()
{
	for (int n = 0; n < mSettings.iterations; ++n)
	{
		for (auto& c : mConstraints)
			c->Apply();
	}

	for (auto& plane : mPlanes)
	{
		for (auto& p : mParticles)
		{
			float temp = 0.0f;

			//	Are we colliding?
			if (Math::Intersect({ p->position, p->radius }, plane, temp))
			{
				auto velocity = p->position - p->lastPosition;
				auto velocityPerpendicular = plane.normal * Math::Dot(velocity, plane.normal);
				auto velocityParallel = velocity - velocityPerpendicular;

				//	Were we colliding?
				if (Math::Intersect({ p->lastPosition, p->radius }, plane, temp))
				{
					const auto newVelocity = velocityParallel * (1.0f - mSettings.drag);
					const float centerDistance = Math::Dot(p->position, plane.normal);
					const float correction = plane.distance - centerDistance + p->radius;
					p->SetPosition(p->position + (plane.normal * correction));
					p->SetVelocity(newVelocity);
				}
				else
				{
					
					const auto newVelocity = (velocityParallel * (1.0f * mSettings.drag))- (velocityPerpendicular * p->bounce);
					p->SetPosition(p->position - velocityPerpendicular);
					p->SetVelocity(newVelocity);
				}
			}
			
		}
	}
	
}