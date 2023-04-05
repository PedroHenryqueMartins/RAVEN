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
		RAVEN::Graphics::ToonEffect mToonEffect;
		RAVEN::Graphics::OutlineEffect mOutlineEffect;

		RAVEN::Graphics::RenderTarget mOutlineRenderTarget;
		RAVEN::Graphics::RenderObject mOutlineScreen;

		RAVEN::Graphics::RenderObject mGroundPlane;
		std::vector<RAVEN::Graphics::RenderGroup> mMawMonsterRG;

		RAVEN::Graphics::ModelID mMawMonsterModelID;
		RAVEN::Graphics::Animator mMawMonsterAnimator;

		int animationIndex = 1;
		int animSize = 0;

		float mAnimationTickValue = 0.0f;

		bool mDrawSkeleton = false;
		bool mPause = false;
		bool mUseToon = false;

	};
}