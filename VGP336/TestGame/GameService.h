#pragma once

#include "TypeIds.h"
#include "RAVEN/Inc/RAVEN.h"


class GameService final : public RAVEN::Service
{
public:
	SET_TYPE_ID(ServiceId::Game);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	std::vector<RAVEN::GameObjectHandle> mHandles;
};