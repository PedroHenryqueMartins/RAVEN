#pragma once

#include "Component.h"

namespace RAVEN
{
	class TransformComponent;

	class GroundPlaneComponent final : public RAVEN::Component
	{
	public:
		SET_TYPE_ID(ComponentId::GroundPlane);

		void Initialize() override;
		void Terminate() override;

		const float& GetSize() const { return mSize; }
		void SetSize(float value) { mSize = value; }

		const Graphics::RenderObject& GetGroundRenderObject() const { return mGroundPlane; }
	private:

		const TransformComponent* mTransformComponent = nullptr;
		Graphics::RenderObject mGroundPlane;

		float mSize = 10.0f;
	};
}
