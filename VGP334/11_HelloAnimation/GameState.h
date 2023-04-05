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
		RAVEN::Graphics::ShadowEffect mShadowEffect;

		RAVEN::Graphics::RenderObject mBall;
		RAVEN::Graphics::RenderObject mGroundPlane;
		RAVEN::Graphics::RenderObject mDuck;

		RAVEN::Graphics::RenderObject mHydroPlane;
		std::vector<std::unique_ptr<RAVEN::Graphics::RenderObject>> mPaladin;

		RAVEN::Graphics::Animation mDuckAnimation;
		float mDuckAnimationTime = 0.0f;

		RAVEN::Graphics::Animation mPlaneAnimation;
		float mPlaneAnimationTime = 0.0f;

		RAVEN::Graphics::Animation mBallAnimation;
		float mBallAnimationTime = 0.0f;

		bool mPause = false;

	};
}