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

		RAVEN::Physics::PhysicsWorld mPhysicsWorld;

		Math::Vector3 particle1Pos = Math::Vector3::Zero;
		Math::Vector3 particle2Pos = Math::Vector3::Zero;

		RAVEN::Graphics::StandardEffect mStandardEffect;
		RAVEN::Graphics::ShadowEffect mShadowEffect;

		RAVEN::Graphics::Model mModel;
		RAVEN::Graphics::RenderGroup mModelRG;

		RAVEN::Graphics::DirectionalLight mDirectionalLight;

		std::vector<RAVEN::Physics::Tetrahedron> mTetrahedrons;
		std::vector<RAVEN::Physics::Cube> mCubes;
		std::vector<RAVEN::Physics::Particle*> mCloth;

		bool mPause = false;
	};
}