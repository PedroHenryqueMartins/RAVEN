#pragma once

#include "RAVEN/Inc/RAVEN.h"

enum class ComponentId
{
	Template = static_cast<uint32_t>(RAVEN::ComponentId::Count)
};

enum class ServiceId
{
	Game = static_cast<uint32_t>(RAVEN::ServiceId::Count),
	Preload
};