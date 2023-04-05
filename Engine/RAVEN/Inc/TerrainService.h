#pragma once

#include "Service.h"

namespace RAVEN
{
	class TerrainService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Terrain);

		void Terminate() override;

		void DebugUI() override;

		void LoadTerrain(const char* fileName, float maxHeight);
		void LoadTexture(const char* fileName, uint32_t index);

		float GetHeight(const Math::Vector3& position) const;

		const Graphics::RenderObject& GetTerrainRenderObject() const { return mTerrainRenderObject; }

	private:
		Graphics::Terrain mTerrain;
		Graphics::RenderObject mTerrainRenderObject;
	};
}