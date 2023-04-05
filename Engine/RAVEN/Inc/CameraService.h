#pragma once

#include "Service.h"

namespace RAVEN
{
	class CameraComponent;

	class CameraService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Camera);

		const Graphics::Camera& GetCurrentCamera() const;

	private:
		friend class CameraComponent;

		void Register(const CameraComponent* cameraComponent);
		void Unregister(const CameraComponent* cameraComponent);

		std::vector<const CameraComponent*> mCameraEntries;
		const CameraComponent* mCurrentCamera = nullptr;
	};
}
