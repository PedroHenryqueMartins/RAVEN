#pragma once

#include <RAVEN/Inc/RAVEN.h>
#include "RavenCharacterController.h"

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

		RAVEN::Graphics::RenderObject mGroundPlane;
		RAVEN::Graphics::RenderGroup mRenderGroup;

		RAVEN::Graphics::ModelID mHeroModelID;
		RAVEN::Graphics::Animator mHeroAnimator;

		RavenCharacterController mCharController;

		bool mDrawSkeleton = false;
		bool mPause = false;

	};
}