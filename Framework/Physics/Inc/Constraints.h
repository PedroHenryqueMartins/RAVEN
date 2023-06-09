#pragma once

#include "Particle.h"

namespace RAVEN::Physics
{
	class Constraint
	{
	public:
		virtual ~Constraint() = default;

		virtual void Apply() const = 0;
		virtual void DebugDraw() const {}
	};

	class FixedConstraint : public Constraint
	{
	public:
		FixedConstraint(Particle* p);
		FixedConstraint(Particle* p, const Math::Vector3& position);

		void Apply() const override;
		void DebugDraw() const override;

	protected:
		Particle* mParticle;
		Math::Vector3 mPosition;
	};

	class Spring : public Constraint
	{
	public:
		Spring(Particle* a, Particle* b, float restLength = 0.0f);

		void Apply() const override;
		void DebugDraw() const override;

	protected:
		Particle* mParticleA;
		Particle* mParticleB;
		float mRestLength;
	};
}