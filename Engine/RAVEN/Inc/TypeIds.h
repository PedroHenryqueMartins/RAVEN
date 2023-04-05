#pragma once

#include "Common.h"

namespace RAVEN
{
	enum class ComponentId
	{
		Animator,
		Collider,
		Camera,
		FPSCameraController,
		GroundPlane,
		Model,
		ThirdPersonCameraController,
		Transform,
		Sight,
		Count
	};

	enum class ServiceId
	{
		Camera,
		Collision,
		Render,
		Terrain,
		Count
	};

	enum EventId : uint32_t
	{
		CollisionEnter,
		CollisionStay,
		CollisionExit,
		SightEnter,
		SightStay,
		SightExit,
		Count,
	};
}

#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override {return StaticGetTypeId(); }