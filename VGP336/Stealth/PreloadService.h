#pragma once

#include "TypeIds.h"
#include "RAVEN/Inc/RAVEN.h"


class PreloadService final : public RAVEN::Service
{
public:
	SET_TYPE_ID(ServiceId::StealthGame);

	void Initialize() override;
};