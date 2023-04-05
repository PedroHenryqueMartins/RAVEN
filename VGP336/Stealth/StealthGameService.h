#pragma once

#include "TypeIds.h"
#include "RAVEN/Inc/RAVEN.h"
#include "PlayerControllerComponent.h"

class StealthGameService final : public RAVEN::Service
{
public:
	SET_TYPE_ID(ServiceId::StealthGame);

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

	void GameOver();
	const bool& isGameOver() const { return mGameOver; }

private:
	std::vector<RAVEN::GameObjectHandle> mHandles;

	RAVEN::Graphics::RenderObject mGround;
	bool mShowDebugDraw = false;
	bool mGameOver = false;

	float mWaitTime = 0.0f;
};