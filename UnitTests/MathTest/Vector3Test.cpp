#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace RAVEN::Math;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		
		

		TEST_METHOD(TestConstructor)
		{
			Vector3 v0;
			Assert::AreEqual(v0.x, 0.0f);
		}

		TEST_METHOD(TestConstructor2)
		{
			Vector3 v0(2.0f);
			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 2.0f);
			Assert::AreEqual(v0.z, 2.0f);
		}

		TEST_METHOD(TestConstructor3)
		{
			Vector3 v0(2.0f, 4.0f, 6.0f);
			Assert::AreEqual(v0.x, 2.0f);
			Assert::AreEqual(v0.y, 4.0f);
			Assert::AreEqual(v0.z, 6.0f);
		}

		TEST_METHOD(TestValuesZero)
		{
			Vector3 v0 = Vector3::Zero;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);
		}

		TEST_METHOD(TestValuesOne)
		{
			Vector3 v0 = Vector3::One;
			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 1.0f);
		}

		TEST_METHOD(TestValuesXAxis)
		{
			Vector3 v0 = Vector3::XAxis;
			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);
		}

		TEST_METHOD(TestValuesYAxis)
		{
			Vector3 v0 = Vector3::YAxis;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 0.0f);
		}

		TEST_METHOD(TestValuesZAxis)
		{
			Vector3 v0 = Vector3::ZAxis;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 1.0f);
		}

		TEST_METHOD(TestNegation)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			v0 = -v0;
			Assert::AreEqual(v0.x, -1.0f);
			Assert::AreEqual(v0.y, -2.0f);
			Assert::AreEqual(v0.z, -3.0f);
		}

		TEST_METHOD(TestAddition)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(1.0f, -2.0f, 4.0f);
			Vector3 v2 = v0 + v1;
			Assert::AreEqual(v2.x, 2.0f);
			Assert::AreEqual(v2.y, 0.0f);
			Assert::AreEqual(v2.z, 7.0f);
		}

		TEST_METHOD(TestSubtraction)
		{
			Vector3 v0(1.0f, 2.0f, -3.0f);
			Vector3 v1(1.0f, -2.0f, 4.0f);
			Vector3 v2 = v0 - v1;
			Assert::AreEqual(v2.x, 0.0f);
			Assert::AreEqual(v2.y, 4.0f);
			Assert::AreEqual(v2.z, -7.0f);
		}

		TEST_METHOD(TestMultiply)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1 = v0 * 4.0f;
			Assert::AreEqual(v1.x, 4.0f);
			Assert::AreEqual(v1.y, 8.0f);
			Assert::AreEqual(v1.z, 12.0f);
		}

		TEST_METHOD(TestDivide)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1 = v0 / 2.0f;
			Assert::AreEqual(v1.x, 0.5f);
			Assert::AreEqual(v1.y, 1.0f);
			Assert::AreEqual(v1.z, 1.5f);
		}

		TEST_METHOD(TestAddEqual)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(5.0f, 3.0f, 4.0f);
			v0 += v1;
			Assert::AreEqual(v0.x, 6.0f);
			Assert::AreEqual(v0.y, 5.0f);
			Assert::AreEqual(v0.z, 7.0f);
		}

		TEST_METHOD(TestSubEqual)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(5.0f, 3.0f, 4.0f);
			v0 -= v1;
			Assert::AreEqual(v0.x, -4.0f);
			Assert::AreEqual(v0.y, -1.0f);
			Assert::AreEqual(v0.z, -1.0f);
		}

		TEST_METHOD(TestMultiplyEqual)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			v0 *= 3.0f;
			Assert::AreEqual(v0.x, 3.0f);
			Assert::AreEqual(v0.y, 6.0f);
			Assert::AreEqual(v0.z, 9.0f);
		}

		TEST_METHOD(TestDivideEqual)
		{
			Vector3 v0(12.0f, 20.0f, 64.0f);
			v0 /= 4.0f;
			Assert::AreEqual(v0.x, 3.0f);
			Assert::AreEqual(v0.y, 5.0f);
			Assert::AreEqual(v0.z, 16.0f);
		}

		TEST_METHOD(TestDot)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(5.0f, 3.0f, 4.0f);
			float v2 = Dot(v0, v1);
			Assert::AreEqual(v2, 23.0f);

		}

		TEST_METHOD(TestMagnitudeSqr)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			float v2 = MagnitudeSqr(v0);
			Assert::AreEqual(v2, 14.0f);

		}

		TEST_METHOD(TestMangnitude)
		{
			Vector3 v0(1.0f, 2.0f, 2.0f);
			float v2 = Magnitude(v0);
			Assert::AreEqual(v2, 3.0f);

		}

		TEST_METHOD(TestDistanceSqr)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(5.0f, 3.0f, 4.0f);
			float v2 = DistanceSqr(v0, v1);
			Assert::AreEqual(v2, 18.0f);

		}

		TEST_METHOD(TestDistance)
		{
			Vector3 v0(6.0f, 2.0f, 3.0f);
			Vector3 v1(2.0f, 2.0f, 3.0f);
			float v2 = Distance(v0, v1);
			Assert::AreEqual(v2, 4.0f);

		}

		TEST_METHOD(TestNormalize)
		{
			Vector3 v0(1.0f, 2.0f, 2.0f);
			Vector3 v2 = Normalize(v0);
			Assert::AreEqual(v2.x, 0.3333333333f);
			Assert::AreEqual(v2.y, 0.6666666666f);
			Assert::AreEqual(v2.z, 0.6666666666f);

		}

		TEST_METHOD(TestCross)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(5.0f, 3.0f, 4.0f);
			Vector3 v2 = Cross(v0, v1);
			Assert::AreEqual(v2.x, -1.0f);
			Assert::AreEqual(v2.y, 11.0f);
			Assert::AreEqual(v2.z, -7.0f);

		}

		TEST_METHOD(TestLerp)
		{
			Vector3 v0(1.0f, 2.0f, 3.0f);
			Vector3 v1(5.0f, 4.0f, 3.0f);
			Vector3 v2 = Lerp(v0, v1, 2.0f);
			Assert::AreEqual(v2.x, 9.0f);
			Assert::AreEqual(v2.y, 6.0f);
			Assert::AreEqual(v2.z, 3.0f);

		}

	};
}
