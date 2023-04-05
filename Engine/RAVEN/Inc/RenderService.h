#pragma once

#include "Service.h"

namespace RAVEN
{
	class CameraService;
	class TerrainService;

	class AnimatorComponent;
	class ModelComponent;
	class TransformComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;
		void DebugUI() override;
		void Update(float deltaTime) override;
		void Render() override;

		void SetShadowFocus(const Math::Vector3& focus);

	private:
		friend class ModelComponent;

		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

		struct Entry
		{
			const AnimatorComponent* animatorComponent = nullptr;
			const ModelComponent* modelComponent = nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		std::vector<Entry> mRenderEntries;

		const CameraService* mCameraService = nullptr;
		const TerrainService* mTerrainService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::TerrainEffect mTerrainEffect;
		Graphics::ShadowEffect mShadowEffect;

		float mFPS = 0.0f;
	};
}
