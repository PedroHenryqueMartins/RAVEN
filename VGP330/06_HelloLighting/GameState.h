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
		RAVEN::Graphics::TexturingEffect mTexturingEffect;

		RAVEN::Graphics::RenderObject mSkydome;
		RAVEN::Graphics::RenderObject mEarth;
		RAVEN::Graphics::RenderObject mCloud;

	};
}

