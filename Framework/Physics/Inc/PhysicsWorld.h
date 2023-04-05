#pragma once

#include "Constraints.h"
#include "Particle.h"

namespace RAVEN::Physics
{

	struct Tetrahedron
	{
		Particle *a, *b, *c, *d;

		Math::Vector3 GetLook() const;
		Math::Vector3 GetRight() const;
		Math::Vector3 GetUp() const;
		Math::Vector3 GetPosition() const;

		Math::Matrix4 GetWorldMatrix() const;

		void SetPosition(const Math::Vector3& pos);
		void SetVelocity(const Math::Vector3& velocity);
	};

	struct Cube
	{
		Particle *a, *b, *c, *d,
				 *e, *f, *g, *h;


		//	Cube Draw
		// 
		//      E--------------F
		//	   ||              | 
		//	  | |             || 
		//   |  |            | |
		//  |   |           |  |
		// |    |          |   |
		// D--------------C	   |
		// |    |         |    |
		// |    H---------|----G
		// |    |         |    |
		// |   |          |   |
		// |  |           |  |
		// | |            | |
		// ||             ||
		// A--------------B


		Math::Vector3 GetLook() const;
		Math::Vector3 GetRight() const;
		Math::Vector3 GetUp() const;
		Math::Vector3 GetPosition() const;

		Math::Matrix4 GetWorldMatrix() const;

		void SetPosition(const Math::Vector3& pos);
		void SetVelocity(const Math::Vector3& velocity);

	};

	struct Cloth
	{
		Particle *a;

		void SetPosition(const Math::Vector3& pos);
		void SetVelocity(const Math::Vector3& velocity);
	};

	class PhysicsWorld
	{
	public:

		struct Settings
		{
			Math::Vector3 gravity{ 0.0f, -9.8f, 0.0f };
			Math::Vector3 wind{ 0.0f, 0.0f, 0.0f };
			bool useWind;
			float timeStep = 1.0f / 60.0f;
			float drag = 0.0f;
			int iterations = 1;
		};

		void Initialize(Settings setings = {});

		void Update(float deltaTime);
		void DebugDraw() const;
		void DebugUI();

		//	For simulation
		template <class... Args>
		Particle* AddParticle(Args&&... args)
		{
			return mParticles.emplace_back(std::make_unique<Particle>(std::forward<Args>(args)...)).get();
		}


		template <class ConstraintType, class... Args>	// Variadic template Arguments
		//	This is not a R reference value because we dont have a specific type. A R value reference is a reference that we can move from, steal memory from.
		//	When used with a templetized, its called a forwarding reference.
		//	int& - l-value reference
		//	int&& - r-value reference
		//	T&& - forwardin reference, can be &, or &&. But is resolved as how it comes in. (don't know if it will be a l-value or a r-value reference, what it can be it will depend on the call side).
		ConstraintType* AddConstraint(Args&&... args)	// Forwarding Reference
		{
			auto& newConstraint = mConstraints.emplace_back(std::make_unique<ConstraintType>(std::forward<Args>(args)...));	// Perfect forwarding.
			return static_cast<ConstraintType*>(newConstraint.get());
		}

		Tetrahedron AddTetrahedron();
		Cube AddCube();

		//	For Environment
		void AddPlane(const Math::Plane& plane);

		void Clear();

	private:

		void AccumulateForces(); //	Gather all the forces.
		void Integrate();
		void SatisfyConstraints();

		std::vector<std::unique_ptr<Constraint>> mConstraints;
		std::vector<std::unique_ptr<Particle>> mParticles;

		std::vector<Math::Plane> mPlanes;

		Settings mSettings;
		float mTimer = 0.0f;
		bool mPause = false;
	};
}