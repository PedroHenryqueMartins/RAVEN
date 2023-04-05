#pragma once

#include "Common.h"

namespace RAVEN::Physics
{
	//	Verlet Integration - velocity-less, numerically stable
	struct Particle
	{
		//	The first three are for controlling movement, motion
		Math::Vector3 position = Math::Vector3::Zero;
		Math::Vector3 lastPosition = Math::Vector3::Zero;
		Math::Vector3 acceleration = Math::Vector3::Zero;

		//	Radius is for collision detection, inverse mass is used to prevent usage of division so inverse mass (1 / m) just multiply to get acceleration ( a = f * 1/m).
		//	If mass is 0 would be a problem because it will be dividing by zero which can't happen cause every mass occupy space in the world.
		//	If inverseMass is zero, mass is infinite (1 / m = 0), means an object is static, hence an object is so heavy that applying a force has no effect on it. Certain times you do want objects to be static.
		//	bounce would be used for loss of energy. 1 means no loss of energy. Zero means that mass is completely inelastic.
		float radius = 1.0f;
		float invMass = 1.0f;
		float bounce = 1.0f;

		Particle() = default;
		Particle(float radius, float invMass)
			: radius(radius)
			, invMass(invMass)
		{}

		void SetPosition(const Math::Vector3& pos)
		{
			//	position = lastposition = no motion
			position = pos;
			lastPosition = pos;
		}

		void SetVelocity(const Math::Vector3& velocity)
		{
			lastPosition = position - velocity;
		}
	};
}