#include "Precompiled.h"
#include "CameraService.h"

#include "CameraComponent.h"

using namespace RAVEN;

const Graphics::Camera& CameraService::GetCurrentCamera() const
{
	ASSERT(mCurrentCamera != nullptr, "Camera Services -- No camera initialized.");
	return mCurrentCamera->GetCamera();
}

void CameraService::Register(const CameraComponent* cameraComponent)
{
	mCameraEntries.push_back(cameraComponent);
	if (mCurrentCamera == nullptr)
		mCurrentCamera = cameraComponent;
}

void CameraService::Unregister(const CameraComponent* cameraComponent)
{
	auto iter = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);

	if (iter != mCameraEntries.end())
	{
		if (mCurrentCamera == *iter)
			mCurrentCamera = nullptr;

		mCameraEntries.erase(iter);

		if (mCurrentCamera == nullptr && !mCameraEntries.empty())
		{
			mCurrentCamera = mCameraEntries.front();
		}
	}
}
