#pragma once

namespace RAVEN::Graphics
{
	// This camera assumes YAxis as up direction. You
	// cannot look straight up or straight down.
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		void SetMode(ProjectionMode mode);

		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);
		void SetLookAt(const Math::Vector3& target);

		// 3 degrees of freedom for translations
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);

		// 2 degrees of freedom for rotations
		void Yaw(float radian);
		void Pitch(float radian);

		// Perspective params
		void SetFov(float fov);
		void SetAspectRatio(float aspectRatio);

		// Orthographic params
		void SetSize(float width, float height);

		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		const Math::Vector3& GetPosition() const { return mPosition; }
		const Math::Vector3& GetDirection() const { return mDirection; }
		float GetFov() const { return mFov; }
		float GetNearPlane() const { return mNearPlane; }
		float GetFarPlane() const { return mFarPlane; }

		Math::Matrix4 GetViewMatrix() const;
		Math::Matrix4 GetProjectionMatrix() const;

		Math::Matrix4 GetPerspectiveMatrix() const;
		Math::Matrix4 GetOrthographicMatrix() const;

	private:
		ProjectionMode mProjectionMode = ProjectionMode::Perspective;

		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		// 0 aspect ratio = use back buffer dimension
		float mFov = 60.0f * Math::Constants::DegToRad;
		float mAspectRatio = 0.0f;

		// 0 width or height = use back buffer dimension
		float mWidth = 0.0f;
		float mHeight = 0.0f;

		float mNearPlane = 0.01f;
		float mFarPlane = 10000.0f;
	};
}

