#pragma once

#include "Common.h"

// Linear Algebra headers
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

// Utils headers
#include "Constants.h"
#include "Random.h"

// Geometry
#include "AABB.h"
#include "OBB.h"
#include "Plane.h"
#include "Sphere.h"

namespace RAVEN::Math
{

	template <class T> constexpr T Min(T a, T b) { return (a > b) ? b : a; }
	template <class T> constexpr T Max(T a, T b) { return (a < b) ? b : a; }
	template <class T> constexpr T Clamp(T value, T min, T max) { return Max(min, Min(max, value)); }
	template <class T> constexpr T Lerp(T a, T b, float t) { return a + (b - a) * t; }
	template <class T> constexpr T Abs(T value) { return (value >= 0) ? value : -value; }
	template <class T> constexpr float Sign(T value) { return (value >= 0) ? 1 : -1; }
	template <class T> constexpr float Sqr(T value) { return value * value; }

	constexpr float Dot(const Vector2& v0, const Vector2& v1)
	{
		return (v0.x * v1.x) + (v0.y * v1.y);
	}

	constexpr float Dot(const Vector3& v0, const Vector3& v1)
	{
		return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
	}

	constexpr float DotXZ(const Vector3& v0, const Vector3& v1)
	{
		return (v0.x * v1.x) + (v0.z * v1.z);
	}

	constexpr float MagnitudeSqr(const Vector2& v)
	{
		return Dot(v, v);
	}

	constexpr float MagnitudeSqr(const Vector3& v)
	{
		 return Dot(v, v);
	}

	constexpr float MagnitudeSqrXZ(const Vector3& v)
	{
		return DotXZ(v, v);
	}

	inline float Magnitude(const Vector2& v)
	{
		return sqrt(MagnitudeSqr(v));
	}

	inline float Magnitude(const Vector3& v) 
	{
		return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	}

	inline float MagnitudeXZ(const Vector3& v)
	{
		return sqrt(MagnitudeSqrXZ(v));
	}

	constexpr float DistanceSqr(const Vector3& a, const Vector3& b) 
	{
		return MagnitudeSqr(a - b);
	}

	constexpr float DistanceSqrXZ(const Vector3& a, const Vector3& b)
	{
		return MagnitudeSqrXZ(a - b);
	}

	inline float Distance(const Vector3& a, const Vector3& b) 
	{
		return sqrt(DistanceSqr(a, b));
	}

	inline Vector2 Normalize(const Vector2& v)
	{
		return v / Magnitude(v);
	}

	inline Vector3 Normalize(const Vector3& v) 
	{
		 return v / Magnitude(v);
	}

	inline Vector3 NormalizeXZ(const Vector3& v)
	{
		return v / MagnitudeXZ(v);
	}

	constexpr Vector3 Cross(const Vector3& u, const Vector3& v) 
	{
		return { ((u.y * v.z) - (u.z * v.y)), ((u.z * v.x) - (u.x * v.z)), ((u.x * v.y) - (u.y * v.x)) };
	}

	constexpr Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t) 
	{
		return v0 + ((v1 - v0) * t);
	}

	constexpr Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		const float invW = 1.0f / ((v.x * m._14) + (v.y * m._24) + (v.z * m._34) + (1.0f * m._44));
		return {
			((v.x * m._11) + (v.y * m._21) + (v.z * m._31) + (1.0f * m._41)) * invW,
			((v.x * m._12) + (v.y * m._22) + (v.z * m._32) + (1.0f * m._42)) * invW,
			((v.x * m._13) + (v.y * m._23) + (v.z * m._33) + (1.0f * m._43)) * invW
		};
	}

	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		return {
			(v.x * m._11) + (v.y * m._21) + (v.z * m._31),
			(v.x * m._12) + (v.y * m._22) + (v.z * m._32),
			(v.x * m._13) + (v.y * m._23) + (v.z * m._33)
		};
	}


	constexpr float Determinant(const Matrix4& m)
	{
		float det = 0.0f;
		det += (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
		det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
		det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
		det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
		return det;
	}

	constexpr Matrix4 Adjoint(const Matrix4& m)
	{
		return
		{
			+(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
			-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
			+(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
			-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

			-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
			+(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
			-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
			+(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

			+(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
			-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
			+(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
			-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

			-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
			+(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
			-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
			+(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
		};
	}

	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		const float determinant = Determinant(m);
		const float invDet = 1.0f / determinant;
		return Adjoint(m) * invDet;
	}

	constexpr Matrix4 Transpose(const Matrix4& m)
	{
		return {
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		};
	}

	inline float Magnitude(const Quaternion& q)
	{
		const float magSqr = (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
		return sqrt(magSqr);
	}

	inline Quaternion Normalize(const Quaternion& q)
	{
		const float magInv = 1.0f / Magnitude(q);
		return q * magInv;
	}

	inline Quaternion Slerp(Quaternion q0, Quaternion q1, float t)
	{
		// Find the dot product
		float dot = (q0.w * q1.w) + (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z);

		// Determine the direction of the rotation.
		if (dot < 0.0f)
		{
			dot = -dot;
			q1 = -q1;
		}
		else if (dot > 0.999f)
		{
			return Normalize(Lerp(q0, q1, t));
		}

		float theta = acosf(dot);
		float sintheta = sinf(theta);
		float scale0 = sinf(theta * (1.0f - t)) / sintheta;
		float scale1 = sinf(theta * t) / sintheta;

		// Perform the slerp
		return Quaternion
		(
			(q0.w * scale0) + (q1.w * scale1),
			(q0.x * scale0) + (q1.x * scale1),
			(q0.y * scale0) + (q1.y * scale1),
			(q0.z * scale0) + (q1.z * scale1)
		);
	}

	inline Vector3 GetRight(const Matrix4& m)
	{
		return { m._11, m._12, m._13 };
	}

	inline Vector3 GetUp(const Matrix4& m)
	{
		return { m._21, m._22, m._23 };
	}

	inline Vector3 GetLook(const Matrix4& m)
	{
		return { m._31, m._32, m._33 };
	}

	inline Vector3 GetTranslation(const Matrix4& m)
	{
		return { m._41, m._42, m._43 };
	}

	bool Intersect(const Vector3& point, const Plane& plane, float& penetrationValue);
	bool Intersect(const Sphere& sphere, const Plane& plane, float& penetrationValue);
	bool IntersectBox(const Vector3& point, const AABB& aabb);
	bool IntersectBox(const Math::AABB& playerAABB, const Math::AABB& otherAABB);
	bool IntersectSphere(const Math::Sphere& a, const Math::Sphere& b);
}