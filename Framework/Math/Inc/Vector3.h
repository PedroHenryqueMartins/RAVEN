#pragma once

namespace RAVEN::Math
{
	struct Vector3
	{
		float x, y, z;

		constexpr Vector3() noexcept : Vector3(0.0f) {}
		constexpr Vector3(float f) noexcept : Vector3(f, f, f) {}
		constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		constexpr Vector3 operator-() const { return Vector3(-x, -y, -z); }
		constexpr Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); } 
		constexpr Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); } 
		constexpr Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); } 
		constexpr Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); } 
		constexpr Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); } 
		

		Vector3& operator +=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; } 
		Vector3& operator -=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; } 
		Vector3& operator *=(float s) { x *= s; y *= s; z *= s; return *this; }			 
		Vector3& operator /=(float s) { x /= s; y /= s; z /= s; return *this; }		
		bool operator ==(const Vector3& v) { x == v.x, y == v.y, z == v.z; return true; }
			 
	};
}