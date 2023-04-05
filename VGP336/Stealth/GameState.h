#pragma once

#include <RAVEN/Inc/RAVEN.h>

namespace RAVEN
{

	class GameState : public AppState
	{
	public:

		void Initialize() override;
		void Termiante() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

		void CreateWorld();

	private:

		RAVEN::GameWorld mGameWorld;
		RAVEN::Graphics::RenderObject mGroundPlane;
		bool mPause = false;
	};
}