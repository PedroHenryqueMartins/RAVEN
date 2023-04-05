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
	private:

		RAVEN::Graphics::Camera mCamera;

		RAVEN::Graphics::DirectionalLight mDirectionalLight;

		RAVEN::Graphics::StandardEffect mStandardEffect;
		RAVEN::Graphics::ToonEffect mToonEffect;
		RAVEN::Graphics::OutlineEffect mOutlineEffect;

		RAVEN::Graphics::RenderTarget mOutlineRenderTarget;
		RAVEN::Graphics::RenderObject mOutlineScreen;

		RAVEN::Graphics::RenderObject mGroundPlane;
		std::vector<std::unique_ptr<RAVEN::Graphics::RenderObject>> mObject;
		std::vector<std::unique_ptr<RAVEN::Graphics::RenderObject>> mObject2;
	};
}