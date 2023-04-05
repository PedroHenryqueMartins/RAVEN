#pragma once

#include "Component.h"

namespace RAVEN
{
	class TransformComponent;
	class SightComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Sight);

		void Initialize() override;
		void Terminate() override;
		
		void DebugUI() override;

		void SetCenter(const Math::Vector3& center) { mCenter = center; }
		void SetExtend(const Math::Vector3& extend) { mExtend = extend; }

		void HandleEvent(const Event& evt) override;

		Math::AABB GetWorldAABB() const;
		Math::AABB GetLocalAABB() const;

		void SetColor(const Graphics::Color& setColor) { mColor = setColor; }
		const Math::Vector3& GetCenter() const { return mCenter; }
		const Math::Vector3& GetExtend() const { return mExtend; }

		const bool& Seen() const { return mSeenPlayer; }

	private:
		const TransformComponent* mTransformComponent = nullptr;
		Math::Vector3 mCenter = Math::Vector3::Zero;
		Math::Vector3 mExtend = Math::Vector3::One;
		Graphics::Color mColor = Graphics::Colors::Blue;
		bool mSeenPlayer = false;
	};
}