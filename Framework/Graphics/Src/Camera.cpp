#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

void Camera::SetMode(ProjectionMode mode)
{
	mProjectionMode = mode;
}

void Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const Math::Vector3& direction)
{
	// Check to prevent our new look direction from being colinear with the Y-axis
	auto dir = Math::Normalize(direction);
	if (Math::Abs(Math::Dot(dir, Math::Vector3::YAxis)) < 0.995f)
		mDirection = dir;
}

void Camera::SetLookAt(const Math::Vector3& target)
{
	mDirection = Math::Normalize(target - mPosition);
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += Math::Vector3::YAxis * distance;
}

void Camera::Yaw(float radian)
{
	Math::Matrix4 matRotate = Math::Matrix4::RotationY(radian);
	mDirection = Math::TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float radian)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	const Math::Matrix4 matRotate = Math::Matrix4::RotationAxis(right, radian);
	const Math::Vector3 newLook = Math::TransformNormal(mDirection, matRotate);
	SetDirection(newLook);
}

void Camera::SetFov(float fov)
{
	const float kMinFov = 10.0f * Math::Constants::DegToRad;
	const float kMaxFov = 150.0f * Math::Constants::DegToRad;
	mFov = Math::Clamp(fov, kMinFov, kMaxFov);
}

void Camera::SetAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
}

void Camera::SetSize(float width, float height)
{
	mWidth = width;
	mHeight = height;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float dx = -Math::Dot(r, mPosition);
	const float dy = -Math::Dot(u, mPosition);
	const float dz = -Math::Dot(l, mPosition);

	return {
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx,  dy,  dz,  1.0f
	};
}

Math::Matrix4 Camera::GetProjectionMatrix() const
{
	return mProjectionMode == ProjectionMode::Perspective ? GetPerspectiveMatrix() : GetOrthographicMatrix();
}

Math::Matrix4 Camera::GetPerspectiveMatrix() const
{
	const float aspectRatio = (mAspectRatio == 0.0f) ? GraphicsSystem::Get()->GetBackBufferAspectRatio() : mAspectRatio;
	const float h = 1 / tan(mFov * 0.5f);
	const float w = h / aspectRatio;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float d = zf / (zf - zn);

	return {
		w,    0.0f, 0.0f,    0.0f,
		0.0f, h,    0.0f,    0.0f,
		0.0f, 0.0f, d,       1.0f,
		0.0f, 0.0f, -zn * d, 0.0f
	};
}

Math::Matrix4 Camera::GetOrthographicMatrix() const
{
	const float width = (mWidth == 0.0f) ? GraphicsSystem::Get()->GetBackBufferWidth() : mWidth;
	const float height = (mHeight == 0.0f) ? GraphicsSystem::Get()->GetBackBufferHeight() : mHeight;
	const float n = mNearPlane;
	const float f = mFarPlane;
	const float w = width;
	const float h = height;

	return Math::Matrix4
	{
		2 / w, 0.0f,  0.0f,        0.0f,
		0.0f,  2 / h, 0.0f,        0.0f,
		0.0f,  0.0f,  1 / (f - n), 0.0f,
		0.0f,  0.0f,  n / (n - f), 1.0f
	};
}
