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

		RAVEN::Graphics::RenderTarget mBaseRenderTarget;
		RAVEN::Graphics::RenderTarget mBloomRenderTarget;

		RAVEN::Graphics::StandardEffect mStandardEffect;
		RAVEN::Graphics::TexturingEffect mTexturingEffect;
		RAVEN::Graphics::PostProcessingEffect mPostProcessingEffect;
		RAVEN::Graphics::GaussianBlurEffect mGaussianBlurEffect;

		RAVEN::Graphics::RenderObject mSkydome;
		RAVEN::Graphics::RenderObject mSun;
		RAVEN::Graphics::RenderObject mEarth;
		RAVEN::Graphics::RenderObject mCloud;
		RAVEN::Graphics::RenderObject mScreenQuad;

		float mEarthDistance = 0.0f;
		float mEarthRotation = 0.0f;
		float mEarthRevolution = 0.0f;
		float mRevolutionIncrement = 0.0f;
		float mCloudRotation = 0.0f;

	};
}

