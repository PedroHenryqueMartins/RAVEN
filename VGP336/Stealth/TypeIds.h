#pragma once

#include "RAVEN/Inc/RAVEN.h"

enum class ComponentId
{
	PlayerControllerComponent = static_cast<uint32_t>(RAVEN::ComponentId::Count),
	AIComponent
};

enum class ServiceId
{
	StealthGame = static_cast<uint32_t>(RAVEN::ServiceId::Count)
};